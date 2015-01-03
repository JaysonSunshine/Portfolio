/* Name: Jay Cloyd

   UID: 504209098

   Others With Whom I Discussed Things:

   Other Resources I Consulted:
   
*/

// import lists and other data structures from the Java standard library
import java.util.*;

// a type for arithmetic expressions
interface AExp {
    double eval(); 	                       // Problem 1a
    List<AInstr> compile(); 	               // Problem 1c
}

class Num implements AExp {
    protected double val;

    public Num(double value) {
    	this.val = value;
	}
	public double eval(){
		return this.val;
	}
	public List<AInstr> compile(){
    	List<AInstr> out = new LinkedList<AInstr>();
    	out.add(new Push(val));
    	return out;
    } 	   
}

class BinOp implements AExp {
    protected AExp left, right;
    protected Op op;

    public BinOp(AExp num1, Op op, AExp num2){
    	this.left = num1;
    	this.right = num2;
    	this.op = op;
    }
    public double eval(){
    	return this.op.calculate(this.left.eval(), this.right.eval());
    }
    public List<AInstr> compile(){
    	List<AInstr> out = new LinkedList<AInstr>();
    	for(int i = 0; i < left.compile().size(); i++){
    		out.add(left.compile().get(i));
		}
    	for(int i = 0; i < right.compile().size(); i++){
    		out.add(right.compile().get(i));
		}
	 	out.add(new Calculate(op));
	 	return out;
    } 	
}

	// a representation of four arithmetic operators
enum Op {
    PLUS { public double calculate(double a1, double a2) { return a1 + a2; } },
    MINUS { public double calculate(double a1, double a2) { return a1 - a2; } },
    TIMES { public double calculate(double a1, double a2) { return a1 * a2; } },
    DIVIDE { public double calculate(double a1, double a2) { return a1 / a2; } };

    abstract double calculate(double a1, double a2);
}

// a type for arithmetic instructions
interface AInstr {
	void eval(Stack<Double> s);
}

class Push implements AInstr {
    protected double val;
    public Push(double value){
    	this.val = value;
    }
    public void eval(Stack<Double> s){
    	s.push(this.val);
    }
    public String toString(){
    	return "Push " + val;
    }
}

class Calculate implements AInstr {
    protected Op op;
    public Calculate(Op operation){
    	this.op = operation;
    }
    public void eval(Stack<Double> s){
    	double n;
    	n = s.pop();
    	s.push(this.op.calculate(s.pop(), n));
    }
    public String toString(){
    	return "Calculate " + op;
    }
}

class Instrs {
    protected List<AInstr> instrs;

    public Instrs(List<AInstr> instrs) { this.instrs = instrs; }

    public double eval() {
    	Stack<Double> s = new Stack<Double>();
    	for(int i = 0; i < this.instrs.size(); i++){
    		this.instrs.get(i).eval(s);
    		//System.out.println(s.peek());
    	}
    	return s.pop();
    }  // Problem 1b
}


class CalcTest {
    public static void main(String[] args) {
	    // a test for Problem 1a
	 AExp aexp =
	     new BinOp(new BinOp(new Num(1.0), Op.PLUS, new Num(2.0)),
	 	      Op.TIMES,
	 	      new Num(3.0));
	 System.out.println("aexp evaluates to " + aexp.eval()); // aexp evaluates to 9.0

	// a test for Problem 1b
	 List<AInstr> is = new LinkedList<AInstr>();
	 is.add(new Push(1.0));
	 is.add(new Push(2.0));
	 is.add(new Calculate(Op.PLUS));
	 is.add(new Push(3.0));
	 is.add(new Calculate(Op.TIMES));
	 Instrs instrs = new Instrs(is);
	 System.out.println("instrs evaluates to " + instrs.eval());  // instrs evaluates to 9.0

	// a test for Problem 1c
	 System.out.println("aexp converts to " + aexp.compile());

    }
}

// a type for dictionaries mapping keys of type K to values of type V
interface Dict<K,V> {
    void put(K k, V v);
    V get(K k) throws NotFoundException;
}

class NotFoundException extends Exception {}


// Problem 2a
class DictImpl2<K,V> implements Dict<K,V> {
    protected Node<K,V> root;

    DictImpl2() {
    	this.root = new Empty<K,V>();
   	}

    public void put(K k, V v) {
    	root = root.put(k,v);
    }

    public V get(K k) throws NotFoundException {
    	try {
    		return root.get(k);
    	} catch(NotFoundException e) {
    		throw new NotFoundException(); }
	}
}

interface Node<K,V> {
	//public boolean empty();
	public Node<K,V> put(K k, V v);
	public V get(K k) throws NotFoundException;
}

class Empty<K,V> implements Node<K,V> {
    Empty() {}
    public Node<K,V> put(K k, V v){
    	return new Entry<K,V>(k,v, new Empty<K,V>());
    }
    public V get(K k) throws NotFoundException {
    	throw new NotFoundException();
    }
}

class Entry<K,V> implements Node<K,V> {
    protected K k;
    protected V v;
    protected Node<K,V> next;

    Entry(K k, V v, Node<K,V> next) {
		this.k = k;
		this.v = v;
		this.next = next;
    }
    public Node<K,V> put(K k, V v){
    	if(this.k == k)
    		this.v = v;
    	else
    		next = next.put(k,v);
    	return this;
    }
    public V get(K k) throws NotFoundException{
    	if(this.k == k)
    		return v;
    	else
    		try {
    			return next.get(k);
    		} catch(NotFoundException e) {
    			throw new NotFoundException();
    		}
    }
}


interface DictFun<A,R> {
    R invoke(A a) throws NotFoundException;
}

// Problem 2b
class DictImpl3<K,V> implements Dict<K,V> {
    protected DictFun<K,V> dFun;

    DictImpl3() {
    	dFun = new DictFun<K,V>() {
    		public V invoke(K k) throws NotFoundException {
    			throw new NotFoundException();
    		}
    	};
    }

    public void put(final K k, final V v) {
    	final DictFun<K,V> oldFun = this.dFun;
    	this.dFun = new DictFun<K,V>() {
    		@Override
    		public V invoke(K k1) throws NotFoundException {
    			if(k == k1)
    				return v;
    			else
    				return oldFun.invoke(k1);
    		}
    	};
    }

    public V get(K k) throws NotFoundException {
    	try {
    		return dFun.invoke(k);
    	} catch(NotFoundException e) {
    		throw new NotFoundException();
    	}
    }
}


class Pair<A,B> {
    protected A fst;
    protected B snd;

    Pair(A fst, B snd) { this.fst = fst; this.snd = snd; }

    A fst() { return fst; }
    B snd() { return snd; }
}

// Problem 2c
interface FancyDict<K,V> extends Dict<K,V> {
    void clear();
    boolean containsKey(K k);
    void putAll(List<Pair<K,V>> entries);
}

class FancyDictImpl3<K,V> extends DictImpl3<K,V> implements FancyDict<K,V> {
	//protected DictFun<K,V> dFun;

	FancyDictImpl3(){
		dFun = new DictFun<K,V>() {
    		public V invoke(K k) throws NotFoundException {
    			throw new NotFoundException();
    		}
    	};
    }
    public void clear(){
    	FancyDictImpl3<K,V> newDict =  new FancyDictImpl3<K,V>();
    	this.dFun = newDict.dFun;
    }
    public boolean containsKey(K k){
    	try{
    		this.get(k);
    		return true;
    	} catch(NotFoundException e){
    		return false;
    	}
    	//return true;
    }
    public void putAll(List<Pair<K,V>> entries){
    	for(int i = 0; i < entries.size(); i++){
    		this.put(entries.get(i).fst(), entries.get(i).snd());
    	}
    }
}


class DictTest {
    public static void main(String[] args) {

	// a test for Problem 2a
	 Dict<String,Integer> dict1 = new DictImpl2<String,Integer>();
	 dict1.put("hello", 23);
	 dict1.put("bye", 45);
	 try {
	     System.out.println("bye maps to " + dict1.get("bye")); // prints 45
	     System.out.println("hi maps to " + dict1.get("hi"));  // throws an exception
	 } catch(NotFoundException e) {
	     System.out.println("not found!");  // prints "not found!"
	 }

	// a test for Problem 2b
	 Dict<String,Integer> dict2 = new DictImpl3<String,Integer>();
	 dict2.put("hello", 23);
	 dict2.put("bye", 45);
	 try {
	     System.out.println("bye maps to " + dict2.get("bye"));  // prints 45
	     System.out.println("hi maps to " + dict2.get("hi"));   // throws an exception
	 } catch(NotFoundException e) {
	     System.out.println("not found!");  // prints "not found!"
	 }

	// a test for Problem 2c
	 FancyDict<String,Integer> dict3 = new FancyDictImpl3<String,Integer>();
	 dict3.put("hello", 23);
	 dict3.put("bye", 45);
	 System.out.println(dict3.containsKey("bye")); // prints true
	 dict3.clear();
	 System.out.println(dict3.containsKey("bye")); // prints false

    }
}
