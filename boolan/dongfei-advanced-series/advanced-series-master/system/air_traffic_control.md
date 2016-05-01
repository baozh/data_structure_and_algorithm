DesignAutomatedAirTrafficControlSystem

Please design an automated Air Traffic Control system where all the planes talk to a master ATC server which gives them commands that tell them where to fly and when to take off or land.
This question is good because you can either expand it or constrict it based on the skill of the candidate. It has no wrong or right answer, its purpose is to see how creative the candidate is and how they approach a problem and determine the requirements for a solution.
I often start out by asking them how the planes talk to the server on the ground and what kind of info would they be sending to the server so that it can make the best decisions.
Useful info that the plane is going to want to send to the server:

   * plane id (flight # / call sign)
   * lat/long coordinates
   * altitude
   * heading (what direction the plane is flying in)
   * speed
   * fuel consumption per (second|minute|hour)
   * amount of fuel left
   * point of origin for the flight
   * destination of the flight
   * if an emergency is taking place, what kind of emergency is it
   * if waiting to land, amount of time spent in holding pattern around airfield
   * if waiting to take off, amount of time spent in line waiting to take off

It's interesting to see what kinds of information that they think the planes should pass to the ground based computer. This part of the question is good for prodding them towards pieces of info that they didn't think about and to see how quickly they catch on to your hints.
I let them spend 5-10 minutes designing the client and server classes and what kind of general data members they have. I don't really care about the data types, I just care about what pieces of information they want to track.
Once they've come up with the design for the classes that are used by the plane and the master ATC computer, I ask them about implementation. How do you model the three dimensional position of a plane inside a data structure? What kind of data structure would make position comparisons between different planes efficient? What if you had to track hundreds or thousands of planes at once?
How do you make sure that two planes won't collide? How do you figure out priority when multiple planes want to land? How do you make sure that a plane doesn't run out of fuel while waiting to land? What if one plane wants to take off and another wants to land at the same time?
What kind of model do you use for the communication between the master server and the different planes? Do the planes subscribe to the server when they enter it's zone and then receive broadcasts that direct them (and all the other planes) what to do? Does the server talk to planes individually instead of over a broadcast? Is there any way to make the system be peer->peer only (i.e. no control computer on the ground)?
This question is really open ended, but if you pay attention to their answers, you can drive them towards any part of the problem and see how they approach the design or implementation. It's a great way to check a candidate's skills in certain areas if you're not sure of their skills in that area. Sometimes I spend the whole time discussing the design of the system with the candidate and we don't even get into implementation, and sometimes the candidate ends up spending all their time explaining how they would represent the position of a plane in a data structure and how they would store and log all the data.
Other things you could ask them about:

   * message latency between planes and server, is it a problem?
   * for certain mission-critical calculations, the space shuttle has multiple computers on board who all attack the same problem. when they find a solution, they compare answers. how might you expand the system so that there are multiple ATC computers in a particular zone? how do the different master computers pass information back and forth? how would they check to make sure that one of their peers wasn't giving out bogus information? what if they figure out that one of their peers is giving out bogus information?

Finally, if they have a CS background, I ask them if they see any similarities between the master ATC computer's logic and other processes/methods/systems that are in the operating systems of most computers. Most smart candidates realize that it's a lot like a process scheduler. This problem makes a nice segue into questions about process priority, what queues are, what is a context switch, stuff like that. I only move on to those questions if they have a CS background and their resume lists that they did some OS work either in school or as part of their job.
