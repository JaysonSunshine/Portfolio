; CS161 Hw3: Sokoban
	; 
	; *********************
	;    READ THIS FIRST
	; ********************* 
	;
	; All functions that you need to modify are marked with 'EXERCISE' in their header comments.
	; Do not modify a-star.lsp.
	; This file also contains many helper functions. You may call any of them in your functions.
	;
	; *Warning*: The provided A* code only supports the maximum cost of 4999 for any node.
	; That is f(n)=g(n)+h(n) < 5000. So, be careful when you write your heuristic functions.
	; Do not make them return anything too large.
	;
	; For Allegro Common Lisp users: The free version of Allegro puts a limit on memory.
	; So, it may crash on some hard sokoban problems and there is no easy fix (unless you buy 
	; Allegro). 
	; Of course, other versions of Lisp may also crash if the problem is too hard, but the amount
	; of memory available will be relatively more relaxed.
	; Improving the quality of the heuristic will mitigate this problem, as it will allow A* to
	; solve hard problems with fewer node expansions.
	; 
	; In either case, this limitation should not significantly affect your grade.
	; 
	; Remember that most functions are not graded on efficiency (only correctness).
	; Efficiency can only influence your heuristic performance in the competition (which will
	; affect your score).
	;  
	;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; General utility functions
	; They are not necessary for this homework.
	; Use/modify them for your own convenience.
	;

	;
	; For reloading modified code.
	; I found this easier than typing (load "filename") every time. 
	;

(defun reload()
  (load "hw3.lsp")
  )

	;
	; For loading a-star.lsp.
	;
(defun load-a-star()
  (load "a-star.lsp"))

	;
	; Reloads hw3.lsp and a-star.lsp
	;
(defun reload-all()
  (reload)
  (load-a-star)
  )

	;
	; A shortcut function.
	; goal-test and next-states stay the same throughout the assignment.
	; So, you can just call (sokoban <init-state> #'<heuristic-name>).
	; 
	;
(defun sokoban (s h)
  (a* s #'goal-test #'next-states h)
  )

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	; end general utility functions
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	; We now begin actual Sokoban code
	;

; Define some global variables
	(setq blank 0)
	(setq wall 1)
	(setq box 2)
	(setq keeper 3)
	(setq star 4)
	(setq boxstar 5)
	(setq keeperstar 6)

; Some helper functions for checking the content of a square
(defun isBlank (v)
  (= v blank)
  )

(defun isWall (v)
  (= v wall)
  )

(defun isBox (v)
  (= v box)
  )

(defun isKeeper (v)
  (= v keeper)
  )

(defun isStar (v)
  (= v star)
  )

(defun isBoxStar (v)
  (= v boxstar)
  )

(defun isKeeperStar (v)
  (= v keeperstar)
  )

; Helper function of getKeeperPosition
(defun getKeeperColumn (r col)
  (cond ((null r) nil)
	(t (if (or (isKeeper (car r)) (isKeeperStar (car r)))
	       col
	     (getKeeperColumn (cdr r) (+ col 1))
	     );end if
	   );end t
	);end cond
  )

; getKeeperPosition (s firstRow)
	; Returns a list indicating the position of the keeper (c r).
	; 
	; Assumes that the keeper is in row >= firstRow.
	; The top row is the zeroth row.
	; The first (right) column is the zeroth column.
	;
(defun getKeeperPosition (s row)
  (cond ((null s) nil)
	(t (let ((x (getKeeperColumn (car s) 0)))
	     (if x
		 ;keeper is in this row
		 (list x row)
		 ;otherwise move on
		 (getKeeperPosition (cdr s) (+ row 1))
		 );end if
	       );end let
	 );end t
	);end cond
  );end defun

; cleanUpList (l)
	; returns l with any NIL element removed.
	; For example, if l is '(1 2 NIL 3 NIL), returns '(1 2 3).
	;
(defun cleanUpList (L)
  (cond ((null L) nil)
	(t (let ((cur (car L))
		 (res (cleanUpList (cdr L)))
		 )
	     (if cur 
		 (cons cur res)
		  res
		 )
	     );end let
	   );end t
	);end cond
  );end 

; Modify this function to return true (t)
	; if and only if s is a goal state of a Sokoban game.
	; (no box is on a non-goal square)
	;
	; Currently, it always returns NIL. If A* is called with
	; this function as the goal testing function, A* will never
	; terminate until the whole search space is exhausted.
	;
	; If there are any boxes, i.e. 2's in the state, then they're not on a goal square
(defun goal-test (s)
	(cond
		((equal nil s) T)
		((listp (car s)) (if (and (goal-test (car s)) (goal-test (cdr s))) T nil))
		(T (if (= 2 (car s)) NIL (goal-test (cdr s))))
  	))
  );end defun

; Any square outside the size of the state is returned as a wall state. Function recurses top row to bottom row and left column
; to right column
(defun get-square (s r c)
	(cond
		((equal nil s) 1)
		((< r 0) 1)
		((< c 0) 1)
		((> r 0) (get-square (cdr s) (- r 1) c))
		((> c 0) (if (equal nil (cdr (car s))) 1 (get-square (cons (cdr (car s)) (cdr s)) r (- c 1))))
		(T (car (car s)))
	))

; Function returns the first c elements from a list
(defun set-helper (l c)
	(cond
		((> c 0) (cons (car l) (set-helper (cdr l) (- c 1))))
		(T nil)
	))

;assumes all rows of same length, per spec. Function recurses through rows until target row is found, then recurses through columns until
; spot found. Then constructs new list with only that element changed
(defun set-square (s r c v)
	(cond
		((equal nil (nthcdr c (car s))) nil)
		((equal nil (nthcdr r s)) nil)
		((< r 0) s)
		((< c 0) s)
		((> r 0) (append (list (car s)) (set-square (cdr s) (- r 1) c v)))
		(T (append (list (append (set-helper (car s) c) (list v) (nthcdr (+ c 1) (car s)))) (cdr s)))
	))
))

; check if a square in state s is a corner by checking two orthogonal directions for wall presence
(defun isCorner (s r c)
	(cond
		((and (= 1 (get-square s (+ r 1) c)) (= 1 (get-square s r (+ c 1)))) T)
		((and (= 1 (get-square s (+ r 1) c)) (= 1 (get-square s r (- c 1)))) T)
		((and (= 1 (get-square s (- r 1) c)) (= 1 (get-square s r (+ c 1)))) T)
		((and (= 1 (get-square s (- r 1) c)) (= 1 (get-square s r (- c 1)))) T)
		(T nil)
))

; D ranges from 0 for up, 1 east, 2 south, 3 west. Attempts to move keeper in that direction if allowable according to rules of Sokoban. If allowed
; updates 2 or 3 squares depending on specific state information
(defun try-move (s D)
	(cond
		((= 0 D) (case (get-square s (- (second (getKeeperPosition s 0)) 1) (first (getKeeperPosition s 0)))
			(0 (set-square (set-square s (- (second (getKeeperPosition s 0)) 1) (first (getKeeperPosition s 0)) 3) (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)) (if (= 3 (get-square s (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)))) 0 4)))
			(1 nil)
			(2 (case (get-square s (- (second (getKeeperPosition s 0)) 2) (first (getKeeperPosition s 0)))
				(0 (set-square (set-square (set-square s (- (second (getKeeperPosition s 0)) 1) (first (getKeeperPosition s 0)) 3) (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)) (if (= 3 (get-square s (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)))) 0 4)) (- (second (getKeeperPosition s 0)) 2) (first (getKeeperPosition s 0)) 2))
				(4 (set-square (set-square (set-square s (- (second (getKeeperPosition s 0)) 1) (first (getKeeperPosition s 0)) 3) (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)) (if (= 3 (get-square s (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)))) 0 4)) (- (second (getKeeperPosition s 0)) 2) (first (getKeeperPosition s 0)) 5))
				(T nil)
				))
			(3 nil)
			(4 (set-square (set-square s (- (second (getKeeperPosition s 0)) 1) (first (getKeeperPosition s 0)) 6) (second (getKeeperPosition s 0)) ( first (getKeeperPosition s 0)) (if (= 3 (get-square s (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)))) 0 4)))
			(5 (case (get-square s (- (second (getKeeperPosition s 0)) 2) (first (getKeeperPosition s 0)))
				(0 (set-square (set-square (set-square s (- (second (getKeeperPosition s 0)) 1) (first (getKeeperPosition s 0)) 6) (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)) (if (= 3 (get-square s (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)))) 0 4)) (- (second (getKeeperPosition s 0)) 2) (first (getKeeperPosition s 0)) 2))
				(4 (set-square (set-square (set-square s (- (second (getKeeperPosition s 0)) 1) (first (getKeeperPosition s 0)) 6) (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)) (if (= 3 (get-square s (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)))) 0 4)) (- (second (getKeeperPosition s 0)) 2) (first (getKeeperPosition s 0)) 5))
				(T nil)
				))
			(6 nil)
			))
		((= 1 D) (case (get-square s (second (getKeeperPosition s 0)) (+ (first (getKeeperPosition s 0)) 1))
			(0 (set-square (set-square s (second (getKeeperPosition s 0)) (+ (first (getKeeperPosition s 0)) 1) 3) (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)) (if (= 3 (get-square s (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)))) 0 4)))
			(1 nil)
			(2 (case (get-square s (second (getKeeperPosition s 0)) (+ (first (getKeeperPosition s 0)) 2))
				(0 (set-square (set-square (set-square s (second (getKeeperPosition s 0)) (+ (first (getKeeperPosition s 0)) 1) 3) (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)) (if (= 3 (get-square s (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)))) 0 4)) (second (getKeeperPosition s 0)) (+ (first (getKeeperPosition s 0)) 2) 2))
				(4 (set-square (set-square (set-square s (second (getKeeperPosition s 0)) (+ (first (getKeeperPosition s 0)) 1) 3) (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)) (if (= 3 (get-square s (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)))) 0 4)) (second (getKeeperPosition s 0)) (+ (first (getKeeperPosition s 0)) 2) 5))
				(T nil)
				))
			(3 nil)
			(4 (set-square (set-square s (second (getKeeperPosition s 0)) (+ (first (getKeeperPosition s 0)) 1) 6) (second (getKeeperPosition s 0)) ( first (getKeeperPosition s 0)) (if (= 3 (get-square s (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)))) 0 4)))
			(5 (case (get-square s (second (getKeeperPosition s 0)) (+ (first (getKeeperPosition s 0)) 2))
				(0 (set-square (set-square (set-square s (second (getKeeperPosition s 0)) (+ (first (getKeeperPosition s 0)) 1) 6) (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)) (if (= 3 (get-square s (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)))) 0 4)) (second (getKeeperPosition s 0)) (+ (first (getKeeperPosition s 0)) 2) 2))
				(4 (set-square (set-square (set-square s (second (getKeeperPosition s 0)) (+ (first (getKeeperPosition s 0)) 1) 6) (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)) (if (= 3 (get-square s (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)))) 0 4)) (second (getKeeperPosition s 0)) (+ (first (getKeeperPosition s 0)) 2) 5))
				(T nil)
				))
			(6 nil)
			))
		((= 2 D) (case (get-square s (+ (second (getKeeperPosition s 0)) 1) (first (getKeeperPosition s 0)))
			(0 (set-square (set-square s (+ (second (getKeeperPosition s 0)) 1) (first (getKeeperPosition s 0)) 3) (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)) (if (= 3 (get-square s (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)))) 0 4)))
			(1 nil)
			(2 (case (get-square s (+ (second (getKeeperPosition s 0)) 2) (first (getKeeperPosition s 0)))
				(0 (set-square (set-square (set-square s (+ (second (getKeeperPosition s 0)) 1) (first (getKeeperPosition s 0)) 3) (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)) (if (= 3 (get-square s (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)))) 0 4)) (+ (second (getKeeperPosition s 0)) 2) (first (getKeeperPosition s 0)) 2))
				(4 (set-square (set-square (set-square s (+ (second (getKeeperPosition s 0)) 1) (first (getKeeperPosition s 0)) 3) (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)) (if (= 3 (get-square s (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)))) 0 4)) (+ (second (getKeeperPosition s 0)) 2) (first (getKeeperPosition s 0)) 5))
				(T nil)
				))
			(3 nil)
			(4 (set-square (set-square s (+ (second (getKeeperPosition s 0)) 1) (first (getKeeperPosition s 0)) 6) (second (getKeeperPosition s 0)) ( first (getKeeperPosition s 0)) (if (= 3 (get-square s (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)))) 0 4)))
			(5 (case (get-square s (+ (second (getKeeperPosition s 0)) 2) (first (getKeeperPosition s 0)))
				(0 (set-square (set-square (set-square s (+ (second (getKeeperPosition s 0)) 1) (first (getKeeperPosition s 0)) 6) (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)) (if (= 3 (get-square s (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)))) 0 4)) (+ (second (getKeeperPosition s 0)) 2) (first (getKeeperPosition s 0)) 2))
				(4 (set-square (set-square (set-square s (+ (second (getKeeperPosition s 0)) 1) (first (getKeeperPosition s 0)) 6) (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)) (if (= 3 (get-square s (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)))) 0 4)) (+ (second (getKeeperPosition s 0)) 2) (first (getKeeperPosition s 0)) 5))
				(T nil)
				))
			(6 nil)
			))
		((= 3 D) (case (get-square s (second (getKeeperPosition s 0)) (- (first (getKeeperPosition s 0)) 1))
			(0 (set-square (set-square s (second (getKeeperPosition s 0)) (- (first (getKeeperPosition s 0)) 1) 3) (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)) (if (= 3 (get-square s (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)))) 0 4)))
			(1 nil)
			(2 (case (get-square s (second (getKeeperPosition s 0)) (- (first (getKeeperPosition s 0)) 2))
				(0 (set-square (set-square (set-square s (second (getKeeperPosition s 0)) (- (first (getKeeperPosition s 0)) 1) 3) (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)) (if (= 3 (get-square s (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)))) 0 4)) (second (getKeeperPosition s 0)) (- (first (getKeeperPosition s 0)) 2) 2))
				(4 (set-square (set-square (set-square s (second (getKeeperPosition s 0)) (- (first (getKeeperPosition s 0)) 1) 3) (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)) (if (= 3 (get-square s (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)))) 0 4)) (second (getKeeperPosition s 0)) (- (first (getKeeperPosition s 0)) 2) 5))
				(T nil)
				))
			(3 nil)
			(4 (set-square (set-square s (second (getKeeperPosition s 0)) (- (first (getKeeperPosition s 0)) 1) 6) (second (getKeeperPosition s 0)) ( first (getKeeperPosition s 0)) (if (= 3 (get-square s (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)))) 0 4)))
			(5 (case (get-square s (second (getKeeperPosition s 0)) (- (first (getKeeperPosition s 0)) 2))
				(0 (set-square (set-square (set-square s (second (getKeeperPosition s 0)) (- (first (getKeeperPosition s 0)) 1) 6) (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)) (if (= 3 (get-square s (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)))) 0 4)) (second (getKeeperPosition s 0)) (- (first (getKeeperPosition s 0)) 2) 2))
				(4 (set-square (set-square (set-square s (second (getKeeperPosition s 0)) (- (first (getKeeperPosition s 0)) 1) 6) (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)) (if (= 3 (get-square s (second (getKeeperPosition s 0)) (first (getKeeperPosition s 0)))) 0 4)) (second (getKeeperPosition s 0)) (- (first (getKeeperPosition s 0)) 2) 5))
				(T nil)
				))
				(6 nil)
			))
	))

; Modify this function to return the list of 
	; sucessor states of s.
	;
	; This is the top-level next-states (successor) function.
	; Some skeleton code is provided below.
	; You may delete them totally, depending on your approach.
	; 
	; If you want to use it, you will need to set 'result' to be 
	; the set of states after moving the keeper in each of the 4 directions.
	; A pseudo-code for this is:
	; 
	; ...
	; (result (list (try-move s UP) (try-move s DOWN) (try-move s LEFT) (try-move s RIGHT)))
	; ...
	; 
	; You will need to define the function try-move and decide how to represent UP,DOWN,LEFT,RIGHT.
	; Any NIL result returned from try-move can be removed by cleanUpList.
	; 
	;
(defun next-states (s)
   (cleanUpList (list (try-move s 0) (try-move s 1) (try-move s 2) (try-move s 3)))
)

; Modify this function to compute the trivial 
	; admissible heuristic.
	;
(defun hur (s)
	0
  )

; Modify this function to compute the 
	; number of misplaced boxes in s.
	;
(defun h1 (s)
	(cond
		((equal nil s) 0)
		((listp (car s)) (+ (h1 (car s)) (h1 (cdr s))))
		(T (if (= 2 (car s)) (+ 1 (h1 (cdr s))) (h1 (cdr s))))
  	))

; looks through a state for boxes. If a box is found (not a box on a goal state) that square is checked if it's a corner. If box in corner, large ;penalty given to heuristic function
(defun cornerBox (s l r c)
	(cond
		((equal nil l) nil)
		((listp (car l)) (not (and (not (cornerBox s (car l) r c)) (not (cornerBox s (cdr l) (+ r 1) c)))))
		(T (if (= 2 (car l)) (if (isCorner s r c) T (cornerBox s (cdr l) r (+ c 1))) (cornerBox s (cdr l) r (+ c 1))))
	))

(defun expand (s r c d)
	(cond
		((= d 0) nil)
		(T (append (list (get-square s (- r 1) c) (get-square s r (+ c 1)) (get-square s (+ r 1) c) (get-square s r (- c 1))) (if (= 1 (get-square s (- r 1) c)) nil (expand s (- r 1) c (- d 1))) (if (= 1 (get-square s r (+ c 1))) nil (expand s r (+ c 1) (- d 1))) (if (= 1 (get-square s (+ r 1) c)) nil (expand s (+ r 1) c (- d 1))) (if (= 1 (get-square s r (- c 1))) nil (expand s r (- c 1) (- d 1)))))
	)
)

(defun min-man-hat-help (l)
	(cond
		((equal nil l) nil)
		((= 4 (car l)) T)
		((= 5 (car l)) T)
		((= 6 (car l)) T)
		(T (min-man-hat-help (cdr l)))
	)
)

(defun min-man-hat (s r c d)
	(cond
		((min-man-hat-help (expand s r c d)) d)
		(T (min-man-hat s r c (+ d 1)))
	)
)

(defun other (s r c)
	(cond
		((= 1 (get-square s r c)) 1000)
		((= 4 (get-square s r c)) 0)
		((= 5 (get-square s r c)) 0)
		((= 6 (get-square s r c)) 0)
		(T (min (+ 1 (other s (+ r 1) c)) (+ 1 (other s (- r 1) c)))) 
))

(defun sum-man-hat (s l r c)
	(cond
		((equal nil l) 0)
		((listp (car l)) (+ (sum-man-hat s (car l) r c) (sum-man-hat s (cdr l) (+ r 1) c)))
		(T (if (= 2 (car l)) (+ (min-man-hat s r c 0) (sum-man-hat s (cdr l) r (+ c 1))) (sum-man-hat s (cdr l) r (+ c 1))))
	)
)

(defun gate-man-hat-help (l)
	(cond
		((equal nil l) nil)
		((= 2 (car l)) T)
		(T (gate-man-hat-help (cdr l)))
	)
)

(defun gate-man-hat (s r c d)
	(cond
		((gate-man-hat-help (expand s r c d)) d)
		(T (gate-man-hat s r c (+ d 1)))
	)
)

(defun gate-dist (s l)
	(- (gate-man-hat s (cadr (getKeeperPosition s 0)) (car (getKeeperPosition s 0)) 0) 1)
)

(defun h0 (s)
	0
)

; For states with boxes in corners, large penalty is applied, otherwise uses h1 heuristic function
; Change the name of this function to h<UID> where
	; <UID> is your actual student ID number. Then, modify this 
	; function to compute an admissible heuristic value of s. 
	; 
	; This function will be entered in the competition.
	; Objective: make A* solve problems as fast as possible.
	; The Lisp 'time' function can be used to measure the 
	; running time of a function call.
	;
(defun hsubmit (s)
	(if (cornerBox s s 0 0) 10 (h1 s))
)

(defun hbest1 (s)
	(if (cornerBox s s 0 0) 100 (sum-man-hat s s 0 0))
)

(defun hbest2 (s)
	(if (cornerBox s s 0 0) 100 (+ (gate-dist s s) (sum-man-hat s s 0 0)))
)



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#| Some predefined problems.
	 | Each problem can be visualized by calling (printstate <problem>). For example, (printstate p1).
	 | Problems are ordered roughly by their difficulties.
	 | For most problems, we also privide 2 additional number per problem:
	 |    1) # of nodes expanded by A* using our next-states and h0 heuristic.
	 |    2) the depth of the optimal solution.
	 | These numbers are located at the comments of the problems. For example, the first problem below 
	 | was solved by 80 nodes expansion of A* and its optimal solution depth is 7.
	 | 
	 | Your implementation may not result in the same number of nodes expanded, but it should probably
	 | give something in the same ballpark. As for the solution depth, any admissible heuristic must 
	 | make A* return an optimal solution. So, the depths of the optimal solutions provided could be used
	 | for checking whether your heuristic is admissible.
	 |
	 | Warning: some problems toward the end are quite hard and could be impossible to solve without a good heuristic!
	 | 
	 |#

;(80,7)
(setq p1 '((1 1 1 1 1 1)
	   (1 0 3 0 0 1)
	   (1 0 2 0 0 1)
	   (1 1 0 1 1 1)
	   (1 0 0 0 0 1)
	   (1 0 0 0 4 1)
	   (1 1 1 1 1 1)))

;(110,10)
(setq p2 '((1 1 1 1 1 1 1)
	   (1 0 0 0 0 0 1) 
	   (1 0 0 0 0 0 1) 
	   (1 0 0 2 1 4 1) 
	   (1 3 0 0 1 0 1)
	   (1 1 1 1 1 1 1)))

;(211,12)
(setq p3 '((1 1 1 1 1 1 1 1 1)
	   (1 0 0 0 1 0 0 0 1)
	   (1 0 0 0 2 0 3 4 1)
	   (1 0 0 0 1 0 0 0 1)
	   (1 0 0 0 1 0 0 0 1)
	   (1 1 1 1 1 1 1 1 1)))

;(300,13)
(setq p4 '((1 1 1 1 1 1 1)
	   (0 0 0 0 0 1 4)
	   (0 0 0 0 0 0 0)
	   (0 0 1 1 1 0 0)
	   (0 0 1 0 0 0 0)
	   (0 2 1 0 0 0 0)
	   (0 3 1 0 0 0 0)))

;(551,10)
(setq p5 '((1 1 1 1 1 1)
	   (1 1 0 0 1 1)
	   (1 0 0 0 0 1)
	   (1 4 2 2 4 1)
	   (1 0 0 0 0 1)
	   (1 1 3 1 1 1)
	   (1 1 1 1 1 1)))

;(722,12)
(setq p6 '((1 1 1 1 1 1 1 1)
	   (1 0 0 0 0 0 4 1)
	   (1 0 0 0 2 2 3 1)
	   (1 0 0 1 0 0 4 1)
	   (1 1 1 1 1 1 1 1)))

;(1738,50)
(setq p7 '((1 1 1 1 1 1 1 1 1 1)
	   (0 0 1 1 1 1 0 0 0 3)
	   (0 0 0 0 0 1 0 0 0 0)
	   (0 0 0 0 0 1 0 0 1 0)
	   (0 0 1 0 0 1 0 0 1 0)
	   (0 2 1 0 0 0 0 0 1 0)
	   (0 0 1 0 0 0 0 0 1 4)))

;(1763,22)
(setq p8 '((1 1 1 1 1 1)
	   (1 4 0 0 4 1)
	   (1 0 2 2 0 1)
	   (1 2 0 1 0 1)
	   (1 3 0 0 4 1)
	   (1 1 1 1 1 1)))

;(1806,41)
(setq p9 '((1 1 1 1 1 1 1 1 1) 
	   (1 1 1 0 0 1 1 1 1) 
	   (1 0 0 0 0 0 2 0 1) 
	   (1 0 1 0 0 1 2 0 1) 
	   (1 0 4 0 4 1 3 0 1) 
	   (1 1 1 1 1 1 1 1 1)))

;(10082,51)
(setq p10 '((1 1 1 1 1 0 0)
	    (1 0 0 0 1 1 0)
	    (1 3 2 0 0 1 1)
	    (1 1 0 2 0 0 1)
	    (0 1 1 0 2 0 1)
	    (0 0 1 1 0 0 1)
	    (0 0 0 1 1 4 1)
	    (0 0 0 0 1 4 1)
	    (0 0 0 0 1 4 1)
	    (0 0 0 0 1 1 1)))

;(16517,48)
(setq p11 '((1 1 1 1 1 1 1)
	    (1 4 0 0 0 4 1)
	    (1 0 2 2 1 0 1)
	    (1 0 2 0 1 3 1)
	    (1 1 2 0 1 0 1)
	    (1 4 0 0 4 0 1)
	    (1 1 1 1 1 1 1)))

;(22035,38)
(setq p12 '((0 0 0 0 1 1 1 1 1 0 0 0)
	    (1 1 1 1 1 0 0 0 1 1 1 1)
	    (1 0 0 0 2 0 0 0 0 0 0 1)
	    (1 3 0 0 0 0 0 0 0 0 0 1)
	    (1 0 0 0 2 1 1 1 0 0 0 1)
	    (1 0 0 0 0 1 0 1 4 0 4 1)
	    (1 1 1 1 1 1 0 1 1 1 1 1)))

;(26905,28)
(setq p13 '((1 1 1 1 1 1 1 1 1 1)
	    (1 4 0 0 0 0 0 2 0 1)
	    (1 0 2 0 0 0 0 0 4 1)
	    (1 0 3 0 0 0 0 0 2 1)
	    (1 0 0 0 0 0 0 0 0 1)
	    (1 0 0 0 0 0 0 0 4 1)
	    (1 1 1 1 1 1 1 1 1 1)))

;(41715,53)
(setq p14 '((0 0 1 0 0 0 0)
	    (0 2 1 4 0 0 0)
	    (0 2 0 4 0 0 0)	   
	    (3 2 1 1 1 0 0)
	    (0 0 1 4 0 0 0)))

;(48695,44)
(setq p15 '((1 1 1 1 1 1 1)
	    (1 0 0 0 0 0 1)
	    (1 0 0 2 2 0 1)
	    (1 0 2 0 2 3 1)
	    (1 4 4 1 1 1 1)
	    (1 4 4 1 0 0 0)
	    (1 1 1 1 0 0 0)
	    ))

;(91344,111)
(setq p16 '((1 1 1 1 1 0 0 0)
	    (1 0 0 0 1 0 0 0)
	    (1 2 1 0 1 1 1 1)
	    (1 4 0 0 0 0 0 1)
	    (1 0 0 5 0 5 0 1)
	    (1 0 5 0 1 0 1 1)
	    (1 1 1 0 3 0 1 0)
	    (0 0 1 1 1 1 1 0)))

;(3301278,76)
(setq p17 '((1 1 1 1 1 1 1 1 1 1)
	    (1 3 0 0 1 0 0 0 4 1)
	    (1 0 2 0 2 0 0 4 4 1)
	    (1 0 2 2 2 1 1 4 4 1)
	    (1 0 0 0 0 1 1 4 4 1)
	    (1 1 1 1 1 1 0 0 0 0)))

;(??,25)
(setq p18 '((0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0)
	    (0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0)
	    (1 1 1 1 1 0 0 0 0 0 0 1 1 1 1 1)
	    (0 0 0 0 0 1 0 0 0 0 1 0 0 0 0 0)
	    (0 0 0 0 0 0 1 0 0 1 0 0 0 0 0 0)
	    (0 0 0 0 0 0 0 0 3 0 0 0 0 0 0 0)
	    (0 0 0 0 0 0 1 0 0 1 0 0 0 0 0 0)
	    (0 0 0 0 0 1 0 0 0 0 1 0 0 0 0 0)
	    (1 1 1 1 1 0 0 0 0 0 0 1 1 1 1 1)
	    (0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0)
	    (0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0)
	    (0 0 0 0 1 0 0 0 0 0 4 1 0 0 0 0)
	    (0 0 0 0 1 0 2 0 0 0 0 1 0 0 0 0)	    
	    (0 0 0 0 1 0 2 0 0 0 4 1 0 0 0 0)
	    ))
;(??,21)
(setq p19 '((0 0 0 1 0 0 0 0 1 0 0 0)
	    (0 0 0 1 0 0 0 0 1 0 0 0)
	    (0 0 0 1 0 0 0 0 1 0 0 0)
	    (1 1 1 1 0 0 0 0 1 1 1 1)
	    (0 0 0 0 1 0 0 1 0 0 0 0)
	    (0 0 0 0 0 0 3 0 0 0 2 0)
	    (0 0 0 0 1 0 0 1 0 0 0 4)
	    (1 1 1 1 0 0 0 0 1 1 1 1)
	    (0 0 0 1 0 0 0 0 1 0 0 0)
	    (0 0 0 1 0 0 0 0 1 0 0 0)
	    (0 0 0 1 0 2 0 4 1 0 0 0)))

;(??,??)
(setq p20 '((0 0 0 1 1 1 1 0 0)
	    (1 1 1 1 0 0 1 1 0)
	    (1 0 0 0 2 0 0 1 0)
	    (1 0 0 5 5 5 0 1 0)
	    (1 0 0 4 0 4 0 1 1)
	    (1 1 0 5 0 5 0 0 1)
	    (0 1 1 5 5 5 0 0 1)
	    (0 0 1 0 2 0 1 1 1)
	    (0 0 1 0 3 0 1 0 0)
	    (0 0 1 1 1 1 1 0 0)))

;(??,??)
(setq p21 '((0 0 1 1 1 1 1 1 1 0)
	    (1 1 1 0 0 1 1 1 1 0)
	    (1 0 0 2 0 0 0 1 1 0)
	    (1 3 2 0 2 0 0 0 1 0)
	    (1 1 0 2 0 2 0 0 1 0)
	    (0 1 1 0 2 0 2 0 1 0)
	    (0 0 1 1 0 2 0 0 1 0)
	    (0 0 0 1 1 1 1 0 1 0)
	    (0 0 0 0 1 4 1 0 0 1)
	    (0 0 0 0 1 4 4 4 0 1)
	    (0 0 0 0 1 0 1 4 0 1)
	    (0 0 0 0 1 4 4 4 0 1)
	    (0 0 0 0 1 1 1 1 1 1)))

;(??,??)
(setq p22 '((0 0 0 0 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0)
	    (0 0 0 0 1 0 0 0 1 0 0 0 0 0 0 0 0 0 0)
	    (0 0 0 0 1 2 0 0 1 0 0 0 0 0 0 0 0 0 0)
	    (0 0 1 1 1 0 0 2 1 1 0 0 0 0 0 0 0 0 0)
	    (0 0 1 0 0 2 0 2 0 1 0 0 0 0 0 0 0 0 0)
	    (1 1 1 0 1 0 1 1 0 1 0 0 0 1 1 1 1 1 1)
	    (1 0 0 0 1 0 1 1 0 1 1 1 1 1 0 0 4 4 1)
	    (1 0 2 0 0 2 0 0 0 0 0 0 0 0 0 0 4 4 1)
	    (1 1 1 1 1 0 1 1 1 0 1 3 1 1 0 0 4 4 1)
	    (0 0 0 0 1 0 0 0 0 0 1 1 1 1 1 1 1 1 1)
	    (0 0 0 0 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#| Utility functions for printing states and moves.
 | You do not need to understand any of the functions below this point.
 |#

	;
	; Helper function of prettyMoves
	; from s1 --> s2
	;
	(defun detectDiff (s1 s2)
	  (let* ((k1 (getKeeperPosition s1 0))
		 (k2 (getKeeperPosition s2 0))
		 (deltaX (- (car k2) (car k1)))
		 (deltaY (- (cadr k2) (cadr k1)))
		 )
	    (cond ((= deltaX 0) (if (> deltaY 0) 'DOWN 'UP))
		  (t (if (> deltaX 0) 'RIGHT 'LEFT))
		  );end cond
	    );end let
	  );end defun

	;
	; Translates a list of states into a list of moves.
	; Usage: (prettyMoves (a* <problem> #'goal-test #'next-states #'heuristic))
	;
	(defun prettyMoves (m)
	  (cond ((null m) nil)
		((= 1 (length m)) (list 'END))
		(t (cons (detectDiff (car m) (cadr m)) (prettyMoves (cdr m))))
		);end cond
	  );

	;
	; Print the content of the square to stdout.
	;
	(defun printSquare (s)
	  (cond ((= s blank) (format t " "))
		((= s wall) (format t "#"))
		((= s box) (format t "$"))
		((= s keeper) (format t "@"))
		((= s star) (format t "."))
		((= s boxstar) (format t "*"))
		((= s keeperstar) (format t "+"))
		(t (format t "|"))
		);end cond
	  )

	;
	; Print a row
	;
	(defun printRow (r)
	  (dolist (cur r)
	    (printSquare cur)    
	    )
	  );

	;
	; Print a state
	;
	(defun printState (s)
	  (progn    
	    (dolist (cur s)
	      (printRow cur)
	      (format t "~%")
	      )
	    );end progn
	  )

	;
	; Print a list of states with delay.
	;
	(defun printStates (sl delay)
	  (dolist (cur sl)
	    (printState cur)
	    (sleep delay)
	    );end dolist
	  );end defun
