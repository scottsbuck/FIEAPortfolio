# FIEAPortfolio
Coding projects and write ups for FIEA

There are 4 source code files included here.  01_hello_SDL.cpp, Order.c, OrderRevamped.cpp, and SkipList.java.
01_hello_SDL.cpp is the source code for a small "game" I made over the summer. It is a snake game and the SnakeGameSample.mkv is a video file showing it running. It still has a few issues. After this semester in AI for Game Progamming I have learned a few things.  The game doesn't run time based, it runs frame based more or less. It has a while loop and whenever a counter hits a certain number, the game updates. This is for a couple of reasons.  When I was first making this game I ran into an issue with the logic of the game.  The way the snake wants to move is by having the segment move to the position of the segment in front of it.  When I first implemented this logic it didn't work because the sprites were so big and were moving a constant small velocity. Basically it "worked" but i couldn't tell because the sprites where just stacking on top of each other.  To remedy this at that moment, I made it so each sprite only updates every 50 loops. The velocity is also the same as the size of the sprite.  This is to prevent the stacking, but the velocity was so fast every loop the segements would fly off the screen, so implemented the updates being only every 50 loops to simulate a velocity of 1. In my game programming class we were taught about using time.delta time as a scalar so that updates aren't frame based. I haven't implemented this yet but id be curious to see if implementing something like that would fix the jitteriness of the game. The snake game has decently extenisve comments going through the various code sections. Something else that should probably be added with further development would be a classs for the object the snake eats, currently it solely exists within the main function, but there is definitely code i could pull out and create functions out of and provide the object member variables.

Order.c is a fun little C project I had in Comp Sci 1.  The idea is that when you run the program it asks you for a new ordering of the alphabet, you then input a list of words and it will the sort the words according the new alphabet you put in.  One parameter you have to put in is the size of the array becuase of how strings in c work. Because strings dont exist in C and you have to use a array of characters.  I think that something that could improve this project is by creating a function that can dynamically allocate size of the string as the user inputs a string. Also to make sure the program doesnt take to long sorting for a large number of words, a MergeSort function is used that keeps the sorting time to O(nlogn).

OrderRevamped.cpp is the project mentioned above but recoded in C++. It is simplified with C++ code and some changes were made for more user friendliness and optimization. I reflect in the comments of the project some of the things that i struggled with while converting the code and also the new things implemented to make the code better. I think the greatest difference would be the use of std::strings in C++ rather than having to use character arrays in C. The memory allocation is not as extensive in C++ as well. Discussed a little aroundthe SkipList, but something else I found interesting is the pass by reference for C++ and Java. When programming in Java almost everything is passed by reference as there are no pointers, but in C++, i actually had a problem recoding this up because i was just passing an object without making it a pointer. The vector wouldn't actually change outside of the function, but once i passed it as a pointer everything started working properly.


SkipList.java was a very difficult assignment in Comp Sci 2. It has comments that explain what each function does. Our professor gave us a general template for the functions required and even suggested some functions that may assist us, but everything else was my code.  The structure of the code means that it can be more or less rewritten in C++ but while rewriting it to show more C++ code I ran into some issues.  Java has a null reference but C++ does not.  You can set something equal to a nullptr, but only if that thing is a ptr. The NULL keyword in C++ is really just 0.  The SkipList code heavily utilizes the idea of checking if something is referencing a null object to then replace the null with a new node. When trying to rewrite this code I was trying to use a vector as its size can dynamically change and has very similar function to that of a ArrayList which is used in the Java code. One thing that was also challenging is that SkipList.java is implemented using Generic data type. The equivalent of this in C++ is templates.  Trying to directly transfer this code to C++ was causing issue with templates, but also not having access to a null object because none of the objects are pointers. If the SkipList was implemented for just a single data type, like Integers, we could use -1 or something to simulate the null pointer.  The current SkipList code currently has no main function and isn't testable without such. 
