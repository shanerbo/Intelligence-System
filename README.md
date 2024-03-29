# cmpt417
### There are 25 files in total.
1. cmpt417Pizza.mzn Main MiniZinc program.
2. instanceGenerator.py A Python script could generate test instances with different congurations.
3. PizzaProblem.cpp A C++11 program which uses Depth First Search and Backtracking algorithm to solve pizza problem. There are 6 more complicated test instances inside of it.
4. README.md A markdown file summarizes everything about this project.
5. test1.dzn Test Instance.
6. test2.dzn Test Instance.
7. test3.dzn Test Instance.
8. test4.dzn Test Instance.
9. test5.dzn Test Instance.
10. test6.dzn Test Instance.
11. test7.dzn Test Instance.
12. test8.dzn Test Instance.
13. test9.dzn Test Instance.
14. test10.dzn Test Instance.
15. test11.dzn Test Instance.
16. test12.dzn Test Instance.
17. test13.dzn Test Instance.
18. test14.dzn Test Instance.
19. test15.dzn Test Instance.
20. test16.dzn Test Instance.
21. test17.dzn Test Instance.
22. test18.dzn Test Instance.
23. test19.dzn Test Instance.
24. test20.dzn Test Instance.
25. cmpt417_report.pdf.









# Requirements

## Part I

### Choose a problem from the five problems from Section 12 of the course notes, "Representing Problems: Examples from the 2015 LP/CP Programming Competition". Further information on these problems can be found on the 2015 LP/CP Programming Contest web page.
#### Choose a solver from the following:
##### MiniZinc
##### The IDP System
##### Minion
#### Spend some time looking at the tutorial or manual of each solver, and some sample problem specifications, to see which one you are most comfortable with.
#### Create a problem specification and set of test instances:
#### Write a problem specification in your chosen system language for your chosen problem.
#### Generate a set of test instances (an absolute minimum of 10 -- for a decision problem 5 satisfiable and 5 unsatisfiable) to help debug specifications for your problem. (You will submit these as part of your project. Try to make them good.)
#### Test your specification for correctness as throughoughly as you can. Use the instances from the contest web page, your own set (see previous item), and others that you may devise to help.
#### Submit, to Coursys, a .zip file of the contents of a directory containing (at least) a short report (.pdf format please), your specification, and your test instances. The report should:
#### Be (for most people) around 2 pages, depending on the length of your specification, choice of test instances, and sizes of your data tables;
#### State your choice of problem and solver system, whether you worked on the decision or optimization version of the problem, and any other decisions or assumptions you made;
#### Give your problem specification.
#### Describe your test instances: how many, how you obtained or constructed them, the rationale behind your choices, and anything else interesting about them;
#### Your settings for parameters of the solver (including saying that you used all default settings, if that is the case);
#### One or more tables of the results (run times, cost function values) of running the solver on your test instances.

#### For Part II of the project, you are to explore some aspect of declarative problem solving, using your solution for Part I as a starting point. This means you should consider some idea about the specification and solving process, ask a question about that idea, carry out an investigation, and write a report about it. Some examples include:
 1. When deciding how to represent a problem, there are several options, and the choice may affect both how we go about writing a specification (and how hard it is to do so), and the performance of various solvers. For example, in solving the (traditional) N-Queens problem, we are asked to arrange n queens on an n-by-n chessboard. We could represent the positioning of the queens on the board by a 2-D array. But we could also observe that there must be a queen in every row, and use a 1-D array in which A[i] is the column in which the i-th queen is placed. Dually, we could have A[i] be the row in which the i-th queen is place. We could even include more than one of these. These choices all lead to different specifications.
 2. When writing a specification, for any given property of solutions to your problem, there are many possible ways of representing the property in the language of a given solver. You may explore different choices, giving consideration to how easy they are to get correct, how clear their meaning is, and especially how they might affect solver performance. Also, you can include redudant constraints, that is constraints which correctly describe properties of the solution but which are not necessary for your specification to be correct. For example, we looked at the fact that there are a number of natural properties satisfied by Latin Squares, and various sub-sets of these properties make correct specifications. One can include any superset of these and have a correct specifation.
 3. Instances of the problems we are looking at range widely in solving times, even among instances that are of essentially the same size. It is natural and interesting to try to understand whether various properties of instances affect solver performance in a predictable way. Similarly, if some instances are particularly hard (escpecially if we don't see any reason that they should be), it is interesting to see if a change in specification or solver setting can improve performance.
#### Your submission should be a .zip file of the contents of a directory that contains your final report, all the files used in the work that you are reporting, and a Readme file that gives a list of the files in the directory. Your report should have three main sections. The first should be your report for Part I (possibly updated). The second should describe the work carried out for Part I. The third should be an appendix, which should contain a list of files (with short descriptions) in the folder you submit, and possibly other things. The main section of the final report should describe the following:
 1. What question are you exploring? Typically, this will begin with some observation that you made about your problem's specifications, solver performance, problem instances, or the process of writing and using your specification. It may be a question about what makes a good specification, about different solvers or solving techniques, or about properties of problem instances, among other things.
 2. How did you go about trying to answer the question? If you developed alternate specifications or insteresting families of problems instances, describe how you went about that, and in what way your choices in doing that amount to an attempt to answer the question.
 3. Details of your experiment or other investigation. For example, what is your experimental set-up (instances, solver, specification, etc.) and what are you measuring, etc. This part should be the most detailed.
 4. Data or other observations.
 5. Discussion: Did your exploration actually go some way toward answering your question? Did it help you understand something interesting about declarative problem solving? What might you do next, if you wanted to continue this study in the future?
