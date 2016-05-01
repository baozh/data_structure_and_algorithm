# Object Resource Pooling
-------
by
Paul King, Senior Software Engineer

## Introduction
The focus of this article is on how pooling resources can be used to gain efficiencies when using sharable objects such as sockets, CORBA objects, database connections, files and the like. The architecture of a simple, re-usable pooling framework will be presented along with a sample implementation.
Today, many free pooling implementations are available and major software vendors incorporate pooling capabilities in their products. In this article I attempt to distill and explain many of the common characteristics and behavior associated with a cache of objects in a resource pool.

## Motivation
If the overhead associated with creating a sharable resource is expensive, that resource is a good candidate for pooling. Rather than take the performance hit associated with creating an object each time it needs to be used, the idea behind pooling is to create the resource in advance and store it away so it can be reused over-and-over.
Even if the overhead associated with creating a resource on the fly is low or acceptable, pooling may be necessary if a limited number of resources are available and sharing is required.
Also, pooling implementations can apply strategies that govern run-time behavior such as load balancing, all-resources-busy situations, and other policies to optimize resource utilization.
Architecture of a Simple Pool Framework
A simple pooling framework can be depicted as follows:

In this framework ConcretePooledObject's extend the java.lang.Object class. This allows objects of any class to be pooled. Some pooling implementations use a marker interface that pooled objects must implement. If that is the case, in order to pool classes that do not implement the marker interface, a Decorator class needs to be created that implements the marker interface. The Decorator class would then wrap the class that does not implement the marker interface.
The Pool interface defines the methods to get a shared object from the pool and release it back to the pool. These paired counter-part methods are often implemented using other names such as get/put, fetch/release, etc.
The AbstractPool class implementation is shown below. Here we define the implementation for the get and release methods and use ArrayList's to store pooled objects that are free and in-use. To guarantee mutually exclusive access and updates to the collection of objects, the get and release methods are synchronized. It is interesting to note that these methods can't be designated synchronized in the Pool interface. This is because methods declared in Java interfaces cannot be qualified as synchronized.
When creating objects to be added to the collection, the AbstractPool uses the PooledObjectFactory it is told to use (for example it can determine the factory from a property). The ConcretePooledObjectFactory is ultimately responsible for creating ConcretePooledObject's.

    import java.util.ArrayList;
    public abstract class AbstractPool implements Pool {        
    	/** Property name that specifies our PooledObjectFactory */        
    	public static final String POOLED_OBJECT_FACTORY_CLASS_NAME =                 "pooledObjectFactoryClassName";        
    	private int defaultPoolSize = 2;        // Pooled objects available and in use.        		private ArrayList freePooledObjects = new ArrayList();        
    	private ArrayList inusePooledObjects = new ArrayList();        // Our instance of the POOLED_OBJECT_FACTORY_CLASS_NAME        
    	private PooledObjectFactory pooledObjectFactory;         
    	
    	public AbstractPool () {            // Fetch the Property name that specifies the             
    		// PooledObjectFactory to use...            
    		String pooledObjectFactoryClassName = System.getProperty(POOLED_OBJECT_FACTORY_CLASS_NAME);            
    		try {                
    			pooledObjectFactory = (PooledObjectFactory)                        				Class.forName(pooledObjectFactoryClassName).newInstance();                
    		} catch (Exception ex) {                
    			// Ouch            
    		}            
    		// Build the pool...            
    		for (int i = 0; i < defaultPoolSize; i++) {                
    			freePooledObjects.add(pooledObjectFactory.newInstance());            
    		}        
    	}        
    	
    	public synchronized Object getPooledObject() {            
    		if (freePooledObjects.size() == 0) {                
    			// Sorry, none available...                
    			return null;            
    		}            
    		// Remove the first...            
    		Object pooledObject = freePooledObjects.remove(0);            
    		// Add it to the in-use list            
    		inusePooledObjects.add(pooledObject);            
    		return pooledObject;        
    	}        
    	
    	public synchronized void releasePooledObject(Object pooledObject) {            
    		freePooledObjects.add(inusePooledObjects.remove(                    
    		inusePooledObjects.indexOf(pooledObject)));        
    		}    
    	}    
    	
Pooling Policies
Pooling implementations often need to enforce policies that dictate run-time behavior. Such policies include:

   * Load Balancing:
Pooled objects can be handed out using an optimization strategy such round-robin, least-used or use some other selection scheme.
   * Low-water/High-water:
A low-water mark is the minimum number of shared objects that will be instantiated when the pool is created. If at any time, all of these objects are in use, a subsequent get request for an object will result in the pool using the associated PooledObjectFactory to create an additional pooled object. This process is allowed until the pool contains the maximum (high-water) number of objects.
   * Blocking vs. No-Wait:
If no objects in a pool are available, a blocking policy allows a client to wait for a specific amount of time hoping that it can claim an object that will be returned to the pool. If the timeout period expires, typically a failure exception is reported back to the client. A no-wait policy immediately reports a failure back to the client if no objects in a pool are available.
   * Failed resource recovery:
If a pooled object becomes unusable due to some type of failure such as disk space exhausted or network problems, then a recovery policy dictates what should be done to fix the object. For performance reasons, it may not be practical to try and synchronously recover a failed object at the point where the failure was detected. Some strategies simply mark the object as failed, effectively removing it from the pool. A monitor thread can then poll for failed objects and attempt to fix them, allowing recovery to occur asynchronously. This eliminates the need to attempt failure recovery at the point in time where the problem was detected. Monitor threads can also proactively perform diagnostic checks on non-failed objects in the pool on a periodic basis (for example if the object has not be successfully used within some period of time) and fix them if errant conditions are detected.
   * Recycler idiom:
JavaWorld Java Tip 78: Recycle broken objects in resource pools focused on a particular approach to reuse broken objects. A Recycler is a form of the Director object that appears in the Builder design pattern. When a failed object is detected, a Recycler examines the object and to tries to replace only the part (or parts) of the object that is broken. This alleviates the cost of removing and creating new pooled objects from scratch. If the Recycler is successful in fixing the failed object, it returns it to the pool.

Concrete Implementation
The following sample code implements a simple pool that supports a blocking policy such that if no objects in the pool are available, then getPooledObject() will wait until an object becomes available. With little effort, such a blocking policy could be implemented in the AbstractPool class. However, for the purposes of this example, we will implement blocking in this concrete class.
The getPooledObject() method attempts to retrieve the next pooled object by calling that method on the super-class, AbstractPool. If no pooled object is available, then the super-class getPooledObject() method returns null. In this case, we execute wait(). The no-arg version of wait is called here for simplicity. This can result in an infinite wait. Industrial strength implementations would allow a timeout to be specified, which if expired, would result in an exception being thrown.
The call to wait() releases the synchronized lock on this instance of the SocketPool object. This will allow a subsequent call to releasePooledObject() to proceed without blocking. The call to releasePooledObject() calls notifyAll(), which will cause the invocation of wait() to complete in any associated thread(s). Then we recursively call our self again, which should succeed in obtaining a free object if contention is weak.

    class BlockingPool extends AbstractPool {        
    	public synchronized Object getPooledObject() {            
    		// Try to aquire. Null return means no objects available.                
    		Object pooledObject = super.getPooledObject();                        
    		if (pooledObject == null) {                
    			try {                    
    				// Wait until notifyAll is called...                    
    				wait();                
    			} catch (InterruptedException ex) {                    // 
    				Ouch                
    			}                    
    			
    			// We received notification that a socket was released.                
    			return getPooledObject(); 
    			// Try again            
    		}            
    		return pooledObject;        
    	}        
    	
    	public synchronized void releasePooledObject(Object pooledObject) {            
    		super.releasePooledObject(pooledObject);            
    		notifyAll();        
    	}    
    }    

To tie all of this together and show the ease of use, we present a simple client class that utilizes our pooling framework. The client intends to pool sockets. As such, we need to create a SocketPooledObjectFactory that simply returns a java.net.Socket instance from its' newInstance() method, following the Factory Method design pattern.



    import java.net.Socket;    
    class SocketPooledObjectFactory implements PooledObjectFactory {        
    	public Object newInstance() {            
    		return new Socket(); // Use the simple constructor for this example        
    	}    
    }
    
Lastly, we present the simple client class that utilizes our pooling framework.

    import java.net.Socket;    
    class SocketPoolClient {        
    	public static void main(String args[]) {            
    		// Set the factory...            	System.setProperty(AbstractPool.POOLED_OBJECT_FACTORY_CLASS_NAME,                     "SocketPooledObjectFactory");            
    		// Create our pool...            
    		BlockingPool socketPool = new BlockingPool();            
    		Socket socketPooledObject = null;                
    		try {                
    			// Get a SocketPooledObject from the pool. Note the need                
    			// to cast from a PooledObject to a SocketPooledObject.                				socketPooledObject = (Socket) socketPool.getPooledObject();                				System.out.println("getPooledObject was a success!");            
    		} finally {                // Release if we were successful in our get...                				if (socketPooledObject != null) 				{                    
    				socketPool.releasePooledObject(socketPooledObject);                    					System.out.println("releasePooledObject was a success!");                
    			}            
    		}        
    	}    
    }
    
        
Once done using the Socket, it is the client's obligation and responsibility to call releasePooledObject(). If for some reason the client does not call releasePooledObject(), then objects will be leaked from the pool and thus cause problems. As such, to be sure that we return the Socket back to the pool, even in the event of an un-handled exception, we place the call to releasePooledObject() within a finally block. From a defensive coding standpoint, the use of finally blocks is a good idea. For a detailed discussion of finally, please refer to the May 2000 Issue of the Java News Brief titled "An integral part of exception handling: finally"
Freely Available Implementations
Among many freely available implementations available, here are a few that I have found useful and interesting.

   * The Apache Server Framework developed by the Jakarta Project includes an Avalon Excalibur component that provides pooling support.
   * Sun Microsystems JDBC 2.0 optional package includes a framework for database Connection and Statement pooling. Such pooling improves performance by caching of database connections and/or statements that may be shared/reused.

Summary
Industrial strength applications need to consider pooling shared resources in order to gain efficiencies. Before implementing your own home-grown framework such as that shown in this article, be sure to see if the package or utility associated with the resource you want to pool already provides a means to facilitate this functionality. You may be surprised, as this behavior is increasingly becoming the norm.
References

   * Avalon Excalibur - http://jakarta.apache.org/avalon/excalibur/pool.html
   * Design Patterns: Elements of Reusable Object-Oriented Software - http://cseng.aw.com/book/0,3828,0201633612,00.html
   * JavaWorld Java Tip 78: Recycle broken objects in resource pools - http://www.javaworld.com/javaworld/javatips/jw-javatip78.html
   * May 2000 Issue of the Java News Brief titled "An integral part of exception handling: finally" - http://www.ociweb.com/jnb/archive/jnbMay2000.html
   * Sun Miccrosystems JDBC 2.0 - http://java.sun.com/j2se/1.3/docs/guide/jdbc/index.html

OCI Educational Services
Object Computing, Inc (OCI) has been providing educational services to clients, industries and universities since 1993. We offer one of the most comprehensive distributed Object Oriented training curricula in the country. These curricula focus on the fundamentals of OO technology; with close to 40 workshops in OOAD, Java, XML, C++/CORBA and Unix/Linux.

Course Catalog | Open Enrollment Schedule
For further information regarding OCI's Educational Services programs, please visit our Educational Services section on the web or contact us at training.
Object Computing, Inc. is a Sun Authorized Java Center in St. Louis, MO and a Member of the Object Management Group, OMG. OCI specializes in distributed computing using object-oriented and web-enabled technologies and provides Consulting, Education and Product Development services to clients nation-wide. For more information contact us in St. Louis, MO (314)579-0066, Tempe, AZ (480)752-0042 or email info.
Inquiries regarding Career Opportunities can be directed to: hr.
The Java News Brief is a monthly newsletter. The purpose and intent of this publication is to advance Java, provide technical value and announce available OCI educational services. To subscribe or unsubscribe from this newsletter, simply click the desired link.
Java and all Java-based marks are trademarks or registered trademarks of Sun Microsystems, Inc. in the United States and other countries.
