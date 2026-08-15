# CS-300
repository for CS 300 class with Professor Rubey

## What was the problem you were solving in the projects for this course?

The main problem I was solving was developing a program for ABCU's Computer Science department that would allow academic advisors to access course information more efficiently. The program needed to load course information from a file, display all courses in alphanumeric order, and allow an advisor to search for an individual course and view its title and prerequisites. Project One focused on planning the program by comparing vectors, hash tables, and binary search trees, while Project Two took that planning and turned it into a working C++ application.

## How did you approach the problem? Consider why data structures are important to understand.

I approached the problem by first looking at how each data structure would handle the actual requirements instead of only looking at which one had the fastest runtime. Vectors, hash tables, and binary search trees each had advantages depending on what the program needed to do. I ultimately chose a binary search tree because its structure worked well for both searching for individual courses and displaying the complete course list in order. Working through the projects also showed me why understanding data structures is important. Choosing a data structure affects not only runtime and memory, but also how the rest of the program needs to be designed.

## How did you overcome any roadblocks you encountered while going through the activities or project?

One of the biggest challenges was moving from pseudocode into a working C++ program and making sure the individual pieces worked together instead of only working independently. I worked through problems by breaking them into smaller sections, testing the logic, and correcting issues as they appeared. I also learned from feedback on Project One, particularly when analyzing runtime. I originally treated some expected or balanced performance as worst-case performance. Revisiting the analysis helped me better understand that a hash table can degrade because of collisions and that a binary search tree that is not self-balancing can become skewed. Debugging the final program also reinforced the importance of paying attention to small details, since something as simple as using a string instead of a character with getline() can prevent otherwise correct logic from compiling.

## How has your work on this project expanded your approach to designing software and developing programs?

This project expanded my approach by reinforcing the value of designing before immediately beginning to code. Writing pseudocode for three different data structures forced me to think about how the same requirements could be solved in different ways and what tradeoffs came with each solution. It also showed me that the best choice is not necessarily whichever structure has the best runtime for one operation. The requirements of the entire program need to be considered. By the time I started Project Two, I already understood how I wanted the program to load, search, and display its data, which made translating the design into C++ more manageable.

## How has your work on this project evolved the way you write programs that are maintainable, readable, and adaptable?

My work on this project has made me more intentional about separating different responsibilities within a program. Instead of putting everything into main(), I used structures, a binary search tree class, and separate functions for tasks such as parsing input, normalizing course numbers, loading and validating data, searching the tree, and printing courses. I also focused more on writing comments that explain why something is being done rather than simply repeating what the code already says. Using descriptive names and separating functionality makes the program easier to read now, but it also makes it easier to modify later without having to rewrite unrelated parts of the program.
