(* EXCEPTIONS *)

(* This is a marker for places in the code that you have to fill in.
   Your completed assignment should never raise this exception. *)
exception ImplementMe of string

(* This exception is thrown when a type error occurs during evaluation
   (e.g., attempting to invoke something that's not a function).
*)
exception DynamicTypeError

(* This exception is thrown when pattern matching fails during evaluation. *)  
exception MatchFailure  

(* EVALUATION *)

(* See if a value matches a given pattern.  If there is a match, return
   an environment for any name bindings in the pattern.  If there is not
   a match, raise the MatchFailure exception.
*)
let rec patMatch (pat:mopat) (value:movalue) : moenv =
  match (pat, value) with
      (* an integer pattern matches an integer only when they are the same constant;
	 no variables are declared in the pattern so the returned environment is empty *)
      (IntPat(i), IntVal(j)) when i=j -> Env.empty_env()
      | (BoolPat(i), BoolVal(j)) when i=j -> Env.empty_env()
      | (WildcardPat, _) -> Env.empty_env()
      | (VarPat(s), j) -> Env.add_binding s j (Env.empty_env())
      | (NilPat, ListVal(NilVal)) -> Env.empty_env()
      | (ConsPat(p1, p2), ListVal(ConsVal(i,j))) -> Env.combine_envs (patMatch p1 i) (patMatch p2 (ListVal(j)))
    | _ -> raise MatchFailure

let rec matchHelp (v: movalue) (l: (mopat * moexpr) list) : (moenv * moexpr) =  
  match l with
    [] -> raise MatchFailure
    | (m, e)::rest -> try (patMatch m v, e) with MatchFailure -> matchHelp v rest

(* Evaluate an expression in the given environment and return the
   associated value.  Raise a MatchFailure if pattern matching fails.
   Raise a DynamicTypeError if any other kind of error occurs (e.g.,
   trying to add a boolean to an integer) which prevents evaluation
   from continuing.
*)
let rec evalExpr (e:moexpr) (env:moenv) : movalue =
  match e with
      (* an integer constant evaluates to itself *)
      IntConst(i) -> IntVal(i)
    | BoolConst(i) -> BoolVal(i)
    | Nil -> ListVal(NilVal)
    | Var(s) -> (try (Env.lookup s env) with Env.NotBound -> raise DynamicTypeError)
    | BinOp(e1, m, e2) -> (match (evalExpr e1 env, m, evalExpr e2 env) with
      (IntVal(i), Plus, IntVal(j)) -> IntVal(i + j)
      | (IntVal(i), Minus, IntVal(j)) -> IntVal(i - j)
      | (IntVal(i), Times, IntVal(j)) -> IntVal(i * j)
      | (IntVal(i), Eq, IntVal(j)) -> BoolVal(i = j)
      | (IntVal(i), Gt, IntVal(j)) -> BoolVal(i > j)
      | (IntVal(i), Cons, ListVal(j)) -> ListVal(ConsVal(IntVal(i), j))
      | _ -> raise DynamicTypeError)
    | Negate(e1) -> (match evalExpr e1 env with
      IntVal(i) -> IntVal(-i)
      | BoolVal(i) -> BoolVal(not i)
      | _ -> raise DynamicTypeError)
    | If(e1, e2, e3) -> (match evalExpr e1 env with
      BoolVal(i) -> if i then evalExpr e2 env else evalExpr e3 env
      | _ -> raise DynamicTypeError)
    | Function(m, e1) -> FunctionVal(None, m, e1, env)
    | FunctionCall(e1, e2) -> (match evalExpr e1 env with
      FunctionVal(s, m, e3, env2) -> match s with 
        None -> evalExpr e3 (Env.combine_envs env2 (patMatch m (evalExpr e2 env)))
        | Some(n) -> evalExpr e3 (Env.combine_envs env2 (Env.add_binding n (evalExpr e1 env) (patMatch m (evalExpr e2 env)))))
    (* It seems we need a helper function for Match. I tried this: Match(e1, (m, e2)::l) -> (match (try (patMatch m (evalExpr e1 env)) with MatchFailure -> evalExpr (Match(e1, l)) env) with
      env1 -> evalExpr e2 (Env.combine_envs env1 env)), but I need to be able to recursively navigate through l*)
    | Match(e1, l) -> (match (matchHelp (evalExpr e1 env) l) with
      (env1, e2) -> evalExpr e2 (Env.combine_envs env1 env))
      (*| _ -> evalExpr (Match(e1, l)) env)
      | _ -> raise DynamicTypeError*)
    (*| _ -> raise DynamicTypeError*)

(* Evaluate a declaration in the given environment.  Evaluation
   returns the name of the variable declared (if any) by the
   declaration along with the value of the declaration's expression.
*)
let rec evalDecl (d:modecl) (env:moenv) : moresult =
  match d with
      Expr(e) -> (None, evalExpr e env)
    | Let(s,e) -> (Some s, evalExpr e env)
    | LetRec(s, p, e) -> (Some s, FunctionVal(Some s, p, e, env))
    (*| _ -> raise (ImplementMe "let and let rec not implemented")*)

