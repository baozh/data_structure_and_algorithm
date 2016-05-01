# The Composite Pattern

## References

   * Design Patterns and Object Oriented Programming, Pages 163--174.

## Aims

   * Develop better understanding of the power of dynamic dispatch.
   * Introduction to a basic design pattern.

## Lecture

In this material, we will explore, via an example, the Composite pattern. Let us say that we are aiming to work with arithmetic expressions built out of operators +,-, *, / and constants. Thus, the expressions that we are interested in are described by the BNF grammar:
e ::= c (constant)   | e1+e2 (addition)   | e1-e2 (subtraction)   | e1*e2 (multiplication)   | e1/e2 (division) 
The capabilities that we desire are as follows:

   * Evaluation via an inorder traversal
   * Preorder traversal for display
   * Postorder traversal for display

We begin by defining an interface that captures the desired functionality:
    public interface Expr {
        void preorder();
        void postorder();
        int evaluate();
    }

Now, for each constructor of arithmetic expressions that we are interested in, we have a subclass that implements this interface. The basic idea is to use dynamic binding as a replacement for conditionals that perform switching based on the kind of expression. Thus, we are looking at a picture of the kind:

Let us begin by only looking at the constructors of the subclasses. The constructors reflect the arity of the arithmetic operators. Thus, we have:
Constant(int i) Plus(Expr lt, Expr rt) Minus(Expr lt, Expr rt) Times(Expr lt, Expr rt) Div(Expr lt, Expr rt) 
Note how the constructors as well as the class diagram show that certain kinds of expressions (all except Const) are composed from subexpressions. Hence the name Composite Pattern.
Even before we write the code that implements these subclasses, the form of the constructors allows us to explore thre representation of expressions that we have achieved.
1 new Constant(1) 1+2 new Plus(new Constant(1), new Constant(2)) 3*4 new Times(new Constant(3), new Constant(4)) (1+2)-(3*4) new Minus(new Plus(new Constant(1), new Constant(2)), new Times(new Constant(3), new Constant(4))) ((1+2)-(3*4))/5 new Div(new Minus(new Plus(new Constant(1), new Constant(2)), new Times(new Constant(3), new Constant(4))), new Constant(5) 
What we are doing is write down the prefix form of the expression tree. The key is that all the above expressions are of type Expr, enabling the following code to typecheck.
    Expr one = new Constant(1);
    Expr onePtwo = new Plus(new Constant(1), new Constant(2));
    Expr threeMfour = new Times(new Constant(3), new Constant(4));
    Expr m = new Minus(onePtwo, threeMfour);
    Expr n = new Div(m,new Constant(5));

Before we proceed further, let us consider each of the subclasses in turn.

    class Constant implements Expr {
        private int v;

        public Constant(int w) { v = w;}

        public int evaluate() { return v; }
        public void preorder() { System.out.println("CONST" + v); }
        public void postorder() { System.out.println("CONST" + v); }
    }
    class Plus implements Expr {
        Expr lt, rt;

        public Plus(Expr l, Expr r) { lt = l; rt =r ; }

        public int evaluate() { return lt.evaluate() + rt.evaluate();}

        public void preorder() {
            System.out.println("PLUS");
            lt.preorder();
            rt.preorder();
        }

        public void postorder() {
            lt.postorder();
            rt.postorder();        System.out.println("PLUS");
        }
    }
    class Minus implements Expr {
        Expr lt, rt;

        public Minus(Expr l, Expr r) { lt = l; rt =r ; }

        public int evaluate() { return lt.evaluate() - rt.evaluate(); }

        public void preorder() {
            System.out.println("MINUS");
            lt.preorder();
            rt.preorder();
        }

        public void postorder() {
            lt.postorder();
            rt.postorder();
            System.out.println("MINUS");
        }
    }
    class Times implements Expr {
        Expr lt, rt;

        public Times(Expr l, Expr r) { lt = l; rt =r ; }

        public int evaluate() { return lt.evaluate() *rt.evaluate();}

        public void preorder() {
            System.out.println("TIMES");
            lt.preorder();
            rt.preorder();
        }

        public void postorder() {
            lt.postorder();
            rt.postorder();
            System.out.println("TIMES");
        }
    }
    class Div implements Expr {
        Expr lt, rt;

        public Div(Expr l, Expr r) { lt = l; rt =r ; }

        public int evaluate() { return lt.evaluate() / rt.evaluate();}

        public void preorder() {
            System.out.println("DIV");
            lt.preorder();
            rt.preorder();
        }

        public void postorder() {
            lt.postorder();
            rt.postorder();
            System.out.println("DIV");
        }
    }

The striking feature of the above code is the total absence of conditionals. Indeed, a main theme of the lectures will be "Inheritance is a succinct way to express conditionals". To make this discussion concrete, considera typical recursive evaluation function that would be written in a standard imperative language, where we have a record type called Node, with a Type function that yields the type of node. The evaluation makes a case-switch based on the type.

    public int evaluate(Expr n) {
        int r;
        if (n  instanceof Constant) {
            r = n.getValue();
        } else if (n instanceof Plus) {
            r = evaluate(n.getLeft()) + evaluate(n.getRight());
        }
        ...
    }

In our presentation, the effect of the conditional is achieved by dynamic dispatch. For example, consider the following code.
    Expr one = new Constant(1);
    Expr onePtwo = new Plus(new Constant(1), new Constant(2));
    Expr threeMfour = new Times(new Constant(3), new Constant(4));
    Expr m = new Minus(onePtwo, threeMfour);
    Expr n = new Div(m,new Constant(5));
    System.out.println(n.evaluate());

We will trace the execution a little to see the way in which dynamic dispatch works.

   * n.evaluate() --- n is an object of Class Div, so the evaluate method invoked is that of Class Div. This call invokes evaluate on m and new Constant(5). Note that the code of class Div (indeed of all subclasses) is not aware of the type of nodes of its two components.
   * new Constant(5).evaluate() --- new Constant(5) is an object of class Const. Thus, the evaluate method invoked is of the subclass Constant and this call returns the value 5.
   * m.evaluate() --- m is an object of class Minus. Thus, the evaluate method invokes in of the subclass Minus.

Issues to explore and think about

   * The presentation above is very fragile with respect to the addition of new operations at the parent class. Thus, any such change has to be carried out systematically across all subclasses. In future lectures, we will examine the use of another design pattern, the Visitor, to localize such changes.
   * Those familiar with recursive data structures and pattern matching constructs in languages such as ML, will recognize the Composite pattern as a way to describe recursive data structures. The Visitor pattern will enable us to build pattern matching in the context of the OO paradigm.
   * In the presentation above, there was no need for parent pointers. In general applications of the Composite pattern, maintaining references from children to parents can facilitate the traversal of the data strucuture --- for a more detailed treatment, see page 166 of Gamma et al.
   * The primary aim of the Composite pattern is to insulate clients from the different kinds of components. in our example, the clients are only aware of Expr, and are insulated from all the different ways of building arithmetic expressions. There is some tension between this aim and the possible variety of node types. For example, in our example, the Constant nodes differ from others in that they are base case; in all other cases, they form inductive steps of expression building, and build bigger expressions out of smaller ones. Thus, in all these latter cases, they need data strcuctures to store and manipulate references to children. In our example, these manipulations are localized to such nodes and are not expressed in the Expr interface. In general, this may not be possible. For a detailed discussion, refer to pages 167--170 of Gamma et al.
   * The use of the Composite pattern to define a language is called the Interpreter pattern.

## Sample Problems
For the purposes of this problem, we will assume that we are given classes Stack and Queue to implement the following interface MyContainer in the expected way.

    interface MyContainer {
        Object first(); // return first element
        void insert(Object x); // insert item into container
        Object delete(); // delete item from container
        boolean isEmpty(); // check if container is empty
    }

We are given the following classes to establish a tree, somewhat along the lines of the last two lectures. The method print() is intended to output the contents of a node, and the children() method is intended creates an iterator that:

   * Leaf Node: Is empty for a leaf node
   * Internal Node: Returns the left child first and the right child next.

    public interface TreeNode {
        void print();
        Iterator children();
    }

    class InternalNode implements TreeNode {
        String content;
        TreeNode lt,rt;

        InternalNode(String o, Tree l, Tree r) {
            content = o; lt =l; rt =r;
        }

        public Iterator children() {
            ArrayList A = new ArrayList();
            A.add(lt); A.add(rt);
            return A.iterator(); 
    // iterator has first element left tree, second element right tree
        }

        public void print() {
            System.out.println(content);
        }
    }

    class LeafNode implements TreeNode {
        Integer content;

        Leaf(Integer o) {
            content = o;
        }

        public Iterator children() {
            return new ArrayList().iterator(); // iterator has no elements
        }

        public void print() {
            System.out.println(content);
        }
    }

Consider the following code in this context.

    public class TreeIterator implements Iterator {
        private MyContainer cont;

        TreeIterator(Tree t, MyContainer C){
            cont = C; cont.insert(t);
        }

        public TreeNode next(){
            TreeNode f = (TreeNode) cont.first();
            cont.delete();

            for (Iterator e = f.children();e.hasNextElement();){
                TreeNode t = (TreeNode)(e.next());
                cont.insert(t);
            }
            return f;
        }

        public boolean hasNextElement(){
            return ! cont.isEmpty();
        }
    }

What is the output of the following code:

    TreeNode one = new LeafNode(new Integer(1)); 
    TreeNode two = new LeafNode(new Integer(2));
    TreeNode three = new LeafNode(new Integer(3));
    TreeNode oneTtwo = new InternalNode("*",one,two);
    TreeNode M = new InternalNode("+",oneTtwo,three);

    TreeIterator i1 = new TreeIterator(T, new Stack());
    while (i1.hasNextElement()){
        Object o = (TreeNode) i1.next(); o.print();
    }TreeIterator i2 = new TreeIterator(T,new Queue());
    while (i2.hasMore()){
        Object o = (TreeNode) i2.next(); o.print();
    }

