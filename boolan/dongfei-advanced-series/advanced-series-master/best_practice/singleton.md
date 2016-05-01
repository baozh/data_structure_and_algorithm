Threading and singleton
a. What is a Singleton?
b. How do you implement it?
c. How does the singleton get initialized? What happens if multiple threads try to get the singleton - how to you ensure that it is only initialized once?
Write Code to implement a Singleton Utility Object
Sample Solutions
Two possible approaches:
Early instantiation
Pros: No synchronization necessary. Cons: Object will be created even if never used.





publicclassSingleton
{
   privatestaticSingleton _instance = newSingleton();
 
   privateSingleton(){}
 
   publicstaticgetInstance()
   {
     return_instance();
   }
}
Lazy-instantiation

   * Simple, expensive solution

Pros: Object is not created unless necessary. Cons: Synchronization needed for all calls to getInstance()





publicclassSingleton
{
   privatestaticSingleton _instance;
 
   privateSingleton(){}
 
   publicsynchronizedstaticgetInstance()
   {
      if(_instance == null)
      {
         _instance = newSingleton();
      }
      return_instance;
   }
}

   * More efficient solution(extra credit)
      * Holder pattern





publicclassSingleton
{
   privatestaticclassSingletonHolder
   {
     publicstaticSingleton _instance = newSingleton();
   }
 
   privateSingleton(){}
 
   publicsynchronizedstaticgetInstance()
   {
      returnSingletonHolder._instance;
   }
}

[Bonus] Is there any way where you could get two instances of a Singleton class loaded within a single VM?
a)   class of which only one instance is or can be created/instantiated. 
b)    A static member variable to hold the single instance; a private constructor to create the single instance (either at class load time or on demand); a way to get the instance such as getInstance() or INSTANCE. 
c)    How does the single instance get created? They should realize that multiple threads trying to get the instance means that you have to synchronize the creation of the instance. The should know both techniques: 


   * "Eager" / "Non-Lazy" initialization: initialized with an initializer expression where the static variable is declared, or with a static code block. [Bonus] They should know that this is thread-safe, because the Java spec guarantees that class initialization (including static members/blocks) is single-threaded.

Lazy initialization: Initialized on first use. They should talk about concurrency control/thread safety: getInstance should be a synchronized method, or have a synchronized block within it. They should not use double-checked locking; if they mention it, it should only be to point out that it is a bad idea.
d)    If you have multiple classloaders (as is typical in a J2EE container), you could potentially have multiple instances of what you think is a Singleton.

