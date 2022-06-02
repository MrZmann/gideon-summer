Part 1:

Amazoff, Amazon's newest competitor, is looking to open their online store. 
They have a website mostly up and running, but they don't have a way of storing 
their inventory. Unfortunately, due to privacy concerns they were unwilling to
release their production code. But luckily they have provided a toy example of 
their requirements in test.cc

Your assignment:

    - Implement stdmap.h using std::map
    - Implement chained.h using std::vector and std::list
    - Answer questions outlined in REPORT.txt

    - Implement Chained.h using templates
    - Implement Open.h

Part 2:
Amazoff developers aren't sure which of your implementations to use in
production. The competition is fierce and frankly the company cannot tolerate
any inefficiencies if they want to stay in business. 

Your assignment:

    - Profile your implementations using getrusage and the perf_event_open
      interface
    - Compare your implementation with standard library implementations
    - Create a test case where chained outperforms open
    - Create a test case where open outperforms chained
    - Graph interesting results and write a short report in LaTeX for the 
      Amazoff development team
