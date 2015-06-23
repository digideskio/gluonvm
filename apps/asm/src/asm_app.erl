%%%-------------------------------------------------------------------
%% @doc asm public API
%% @end
%%%-------------------------------------------------------------------

-module('asm_app').

-behaviour(application).

%% Application callbacks
-export([ start/2
        , stop/1
        , start/0
        ]).

%%====================================================================
%% API
%%====================================================================

start() ->
  %application:start(asm)
  process("test/init.S").

start(_StartType, _StartArgs) ->
  'asm_sup':start_link().

%%--------------------------------------------------------------------
stop(_State) ->
    ok.

%%====================================================================
%% Internal functions
%%====================================================================

-record(compile_state, { %fun_offset :: integer()
                        accum = []
                       , mod
                       }).

%% @doc Loads beam S assembly and runs compile on it
process(Fname) ->
  {ok, Asm} = file:consult(Fname),

  %% Cut away header tags
  Trimmed = lists:dropwhile(fun not_func_header/1, Asm),
  Funs = split_funs(Trimmed, []),
  io:format("Funs ~p~n", [Funs]),

  Mod0 = asm_module:new(init),
  Mod1 = asm_module:set_exports(proplists:get_value(exports, Asm), Mod0),

  CompileState = #compile_state{mod = Mod1},
  #compile_state{accum=Code, mod=Mod2}
    = lists:foldr(fun compile_one_fun/2, CompileState, Funs),
  Mod = asm_module:set_ir(lists:flatten(Code), Mod2),
  io:format("Module ~p~n", [Mod]),
  ok = asm_module:write_ir(Fname ++ ".ir", Mod).

%% @doc Predicate to separate fun headers
not_func_header({function, _, _, _}) -> false;
not_func_header(_) -> true.

%% @doc Splits funs using {function,_,_,_} as first line in each block.
%% Takes Asm with stripped headers; Returns [{fun_name, arity, [code]}]
split_funs([], Accum) -> lists:reverse(Accum);
split_funs([{function, FName, FArity, _} | Code], Accum) ->
  {Fun, Remaining} = lists:splitwith(fun not_func_header/1, Code),
  split_funs(Remaining, [{FName, FArity, Fun} | Accum]).

compile_one_fun({F, Arity, Code}, CState) ->
  CState1 = lists:foldl(fun c_op/2, CState, Code),
  %% First opcode in function is always label, get it
  {label, FLabel} = hd(Code),
  M1 = CState1#compile_state.mod,
  {FunAtomIndex, M2} = asm_module:find_or_create_atom(F, M1),
  M3 = asm_module:add_fun(FunAtomIndex, Arity, FLabel, M2),
  CState1#compile_state{mod=M3}.

%% @doc Compiles individual opcodes to Gluon Intermediate
%% CState = compiler state, namely function base offset for labels
c_op({label, L}, #compile_state{mod=Mod0, accum=Acc}=CState) ->
  Mod1 = asm_module:register_label(L, length(Acc), Mod0),
  CState#compile_state{mod=Mod1};
c_op({func_info, _A1, _A2, _N}, CState=#compile_state{}) -> CState; % NO OP
c_op({line, Props}, CState=#compile_state{mod=Mod0}) ->
  case asm_module:get_option(line_numbers, Mod0) of
     true -> case lists:keyfind(location, 1, Props) of
               false -> CState;
               {location, F, L} ->
                 {Op, Mod1} = asm_op:'LINE'(F, L, Mod0),
                 emit(Op, CState#compile_state{mod=Mod1})
             end;
     false -> CState
  end;
c_op({move, Src, Dst}, CState = #compile_state{mod=Mod0}) ->
  {MoveOp, Mod1} = asm_op:'MOVE'(Src, Dst, Mod0),
  emit(MoveOp, CState#compile_state{mod=Mod1});
c_op({gc_bif, Lbl, _OnFail, Bif, Args, ResultDst}
    , CState=#compile_state{mod=Mod0}) ->
  {PushArgs, Mod1} = lists:foldr(fun fold_push_argument/2
                                , {[], Mod0}
                                , Args),
  BifFlag = case Bif of 1 -> bif; 0 -> non_bif end,
  {CallOp, Mod2} = asm_op:'CALL'(Lbl, length(Args), BifFlag, ResultDst, Mod1),
  emit(PushArgs ++ [CallOp], CState#compile_state{mod=Mod2});
c_op({call_only, Arity, Label}, CState=#compile_state{mod=Mod0}) ->
  {CallOp, Mod1} = asm_op:'TAILCALL'(Arity, Label, Mod0),
  emit(CallOp, CState#compile_state{mod=Mod1});
c_op({call_ext_only, Arity, Label}, CState=#compile_state{mod=Mod0}) ->
  {CallOp, Mod1} = asm_op:'TAILCALL'(Arity, Label, Mod0),
  emit(CallOp, CState#compile_state{mod=Mod1});
c_op({test, Test, Label, Args}, CState=#compile_state{mod=Mod0}) ->
  {TestOp, Mod1} = asm_op:'TEST'(Test, Label, Args, Mod0),
  emit(TestOp, CState#compile_state{mod=Mod1});
c_op(return, CState=#compile_state{}) ->
  emit(asm_op:'RET'(), CState);
c_op(UnkOp, {_Acc, _MState, _CState}) ->
  erlang:error({unknown_op, UnkOp}).
  %{[{unknown, UnkOp} | Acc], MState, CState}.

fold_push_argument(Arg, {Acc, MState}) ->
  {Op, MState1} = asm_op:'PUSH'(Arg, MState),
  {[Op | Acc], MState1}.

emit(Op, CState) when not is_list(Op) -> emit([Op], CState);
emit(Ops, CState = #compile_state{accum = Acc0}) ->
  %% TODO: this is O(N)
  CState#compile_state{accum = Acc0 ++ Ops}.
