SoftwareStandards/CommentingYourCode

< SoftwareStandards
Please read and understand the Disclaimer before reading this page.
Synopsis: This document describes guidelines for writing effective documentation within your source code.
Contents[hide]

   * 1 Introduction
   * 2 Why Documentation Is Important
      * 2.1 Efficiency
      * 2.2 Safety
   * 3 Documentation Style
      * 3.1 General Guidelines
      * 3.2 Why
      * 3.3 What
      * 3.4 How
[edit]Introduction
Documentation is a core part of the Software Engineering process. Recognizing that it is important is part of a broader understanding that software is much more than just code.
There are three components to good software: design, performance, and documentation. Software that's well-designed, performs acceptably, and well-documented, is good software. In this document we focus on how to fulfill the third part of this: how to write documentation within your source code.
[edit]Why Documentation Is Important
Documentation is important in any Engineering discipline for 2 reasons: efficiency and safety.
[edit]Efficiency
It may seem strange to think of documentation in terms of efficiency, but the fact is, if you don't document what you do, you will waste time. Many engineers decide to "save time" on a project by not documenting their source code. This is a bad idea. It rarely saves much time in the short-term (if you document the right things, it doesn't take long to write documentation). In the long term, bad or missing documentation is one of the leading causes of wasted time and frustration.
The reason why is that, fundamentally, software design consists of two things:

   * Experimentation to figure out how to do something
   * Coding to implement the solution

As engineers we usually spend as much or more time experimenting as we do coding. If we don't document the results of these experiments we lose 50% or more of our work over time. We never remember everything we did to figure out why we coded things a certain way. And code, without documentation, does not contain much information about the experiments that led to it. Code describes to a computer how to do something; it describes the end result, not the process that led there. Programming languages are not "experiment description" languages.
You're experimenting far more than you may realize. If you're just fixing a bug, understanding root cause of the bug and duplicating it is an experiment. If you're designing a major new system you have to create a small series of experiments to validate design concepts. Losing the result of these experiments forever is inefficient.
Even bad code is valuable if it's well-commented. The results of the experimentation have been captured and the code can be re-written, if needed, in about 50-75% less time than it would take otherwise.
In summary:

   * Not documenting always leads to wasted time (for you and/or someone else)
   * Badly-written code is still valuable if it's well-documented.

[edit]Safety
During a crisis, someone may need to find out exactly why something is set up the way it is in order to debug the problem. For example, imagine looking at the following uncommented code fragment during a website outage:

drain_input_order_queues();write_to_output_order_queue();
Say you've traced the problem down to the fact that drain_input_order_queues() has some kind of nasty, hard-to-fix bug, and you'd like to just remove it for now to fix the site outage. Without any comments, you have no idea whether it's safe to do that, because you have no idea why the queue is being drained in the first place. So you have to track down the original author (if that data is still available), try to contact them (which may fail), and hope that they remember the code (they often don't). If you cannot contact someone who knows why the code was written that way, you have to waste time tracing through other code and experimenting in order to figure out if your change is safe. Meanwhile time is passing, customers are unhappy, and the company is losing money hand-over-fist. On the other hand, imagine the code actually explained why it made the call:



// We call this first due to a bizarre bug (#186293) in// common_order_handler that makes it much less efficient// at sending data when the input queue is full.drain_input_order_queues();write_to_output_order_queue();
First, you could check whether bug #186293 was fixed. Even if it wasn't, you know there's a chance you can just remove the call since it was added for performance reasons.
[edit]Documentation Style
There are two major aspects of documentation style:

   * Substance: what information is contained in the documentation
   * Appearance: how it looks

Both are important, but the substance is more so. We focus on substance in this document. See the coding standards for each language for guidelines on appearance.
Here are the things that documentation should explain about the source code:

   * Why: Why does the code exist? What is its motivation?
   * What: What does the code accomplish.
   * How: How does the code accomplish it (especially at a design level).

It's important to document all these things. It's also important not to document other things that aren't important. For example, excessive documentation of the obvious gives documentation a bad name; engineers often use this as an excuse not to document their work at all.
Before diving into specifics, let's cover some general guidelines about how to write good documentation for your software.
[edit]General Guidelines
General Guidelines The following are some quick, general guidelines to writing documentation within your software.
Each file should contain a copyright notice.
Documentation should be terse (but complete). Many engineers avoid writing documentation because they feel they would have to write a lot of documentation for it to be useful. Not true! While you should cover the topic thoroughly (e.g., document all the side-effects of a function), keep verbiage to a minimum!
Provide specific examples. Examples help people understand better than general descriptions, much like "a picture says 1000 words". Give specific examples of how your code might be used, or who a typical client of your code might be, or a problem that was really encountered in real-life that led to writing the code, etc.
Assume the reader is fluent in the programming language you're writing in. Don't explain how C/C++/Java/Perl/whatever works in your comments, except when you're using an obscure feature of the language (which you probably ought to avoid in the first place).
Assume the reader knows little about your program or its overall goals. Write your comments for a novice engineer who's trying to get up to speed on your code. It's difficult to know who will be reading your code some day; that would be tantamount to predicting the future. Engineers often underestimate how widely their code will be read.
Refer to external documents. Comments can refer to whatever they need to (a certain page of a book, a webpage, etc) to get the point across without a lot of verbiage. Don't feel restricted to referring only to the other source files within your module. For example, if you're implementing a search algorithm out of Knuth, just refer to the book and page you got it from, don't explain the search algorithm in the comments. Fix up missing or incomplete documentation. If you run across some confusing, undocumented code, fix it up so the next person to look at it doesn't have to waste time trying to figure it out again.
[edit]Why
Documenting why you did things a certain way is essential. The "why" is what much of your experimentation led to. Losing the results of your experimentation is wasteful.
The biggest question other engineers have when looking at code (or even their own) is, "Why is this code here?", or, "Why did they (I) do it this way?", or, "What specific problem does this code solve?" Don't confuse this with, "Why was I personally motivated to write this code?", or, "What personal anguish did I go through to write the code?"
Here are some examples of misguided "why" comments:
Example (bad):

// This is a horrible hackdrain_output_queues();
Example (bad):

// Removing this for now#if 0set_file_expiration_time(0);#endif
Example (mediocre):

// I was up really late and this just seemed to work, had// to get it out the door.drain_output_queues();
Tips for documenting "why?"
Be as specific as possible about why you did something.
Example (bad):

// I did this to fix a bugdrain_output_queues();
Example (mediocre):

// I did this to fix a bug in order processing// codedrain_output_queues();
Example (good):

// The following line is here only to fix// bug #1532687.  It can be removed once the// fix to #1531197 is in place, as it's just// a performance optimization.drain_output_queues();
Example (bad):

// Added to support order processingrun_order_proc_hooks();
Example (good):

// Order processing needs a chance to run// before printing the label, to make VAT// adjustments based on the final shipping// address.run_order_proc_hooks();
Avoid personal commentary. Don't be cute or include personal anecdotes (as a bonus, you won't have to type as much!):
Example (bad):

// This code sucks, but I spent weeks pulling// out my hair trying figure out why nothing// works if I remove this call and could never// figure it out.drain_output_queues();
Example (good):

// Still unsure why, but if you remove it the order// processing code slows down by a factor of// 3x.  Contact cameron@ if you want to// investigate further.drain_output_queues();
Example (bad):

// First I tried not draining the output, then// I went home and was hacking on this// thing forever, fed my dog, thought about it// for a while, pulled out my hair, couldn't// figure it out, and just put this line in// and it seemed to work.drain_output_queues();
Terse statements are often acceptable, if you're truly in a crunch (provided someone can figure still figure out why you did it):
Example (ok):

// Bugfix.  See #1532687 for details.drain_output_queues();
Example (ok):

// Stevens (UNPv1, p. 123) says to do thissetsid();
[edit]What
Documenting what the code does is important. The key point is that you should document what the code does that isn't obvious. It's also important not to document stuff that doesn't help people. If you find yourself writing a lot of comments explaining what the code is doing, you're probably writing needlessly unclear code in the first place.
"What" style comments are for:

   * Documenting what a function does (so someone else can use it)
   * Documenting side-effects
   * Documenting what a block of code does, if it's not obvious (so someone can debug it)

The classic example of a bad "what" comment is:

// Add one to ii++;
The above comment is completely useless because you should assume the reader is familiar with C or C-like syntax.
Here's another example (this time of an "ok" comment):

// We rely on this setting g_output_log_dir as a side-effect.const char *log_file_name_template = log_get_file_name_template();
It's obvious from the code itself what the code does, but what isn't obvious is that log_get_file_name_template has a side-effect, so the side-effect is commented. If it weren't commented, then someone could introduce a nasty bug by removing or moving the call without realizing the full ramifications. Of course, side-effects are usually a sign of bad design. In this case, it would be a good idea to fix the design. However, you might not always be able to do that (e.g., when it's pre-existing code and you don't have enough time to fix it).
[edit]How
Documenting how code works is important if it's algorithmically complex or if it's part of a complex system. "How" style comments are for:

   * Explaining how complex code works
   * Explaining how code fits into a complex system

The underlying goal of "how" style comments is to allow people to be able to modify, debug, or reuse parts of your code. A key to writing this kind of documentation is to avoid re-writing the code in the documentation.
Here's an example of a good comment:

// Use the Elrond-Hubbard algorithm to sort the ASINs// in log(n-2) time.  See Knuth v1, p. 234....hairy code...
Here's a bad one:

  // Process each ASIN.  For each ASIN, we check the validity,  // then shift the asin left 3 bits.  for (i = 0; i < MAX_ASINs; i++) {    check_validity(asin[i]);    hashed_asin = asin[i] << 3;  }
The above example is somewhat contrived, but it's easy to fall into the trap of just writing down the code in English, which is only helpful if you didn't use good variable names. Programmers often fall into two traps here.
Either:
(a) They see a lot of comments like the bad example above and form the opinion that comments are useless as long as they have "descriptive" variable names.
or:
(b) They write a lot of good comments that are only needed because they don't have descriptive variable names.
An example of (a) is the following:

for (asin_number = 0; asin_number <= MAXIMUM_ASIN_NUMBER;            asin_number++) {    ...some really complicated stuff without any comments...}
An example of (b) is the following:

// Loop up to the maximum number of asinsfor (i = 0; i < 259; i++) {  // Send each asin through the order processing stage  asin_op(a[i]);}
