Some things to consider

1) It is assumed that the Car always start from (0,0) 

2) The app will ask initially for the max grid points x and y

3) Passenger Input is through a .txt file .The app reads a "PassengerList.txt" file which should reside in the same folder as
    the executable. 

4)  The format for txt file is simple, for eg "Ian 2 2 4 6", means Passenger name is Ian, his starting pt is (2,2) and end pt is (4,6).One line has only one passenger info


5) At every block time the app will read the file to check for updates. 
 

6) To continue moving the car by one step the user needs to provide "n" as input on std in out cmdline continously.
    This will allow user to recreate "PassengerList.txt" with new passenger information

7) After the first input, the app will read "PassengerList.txt" and will be ready for operation 

8) On each "n" input afterwards, the car will move one block if it has passengers to pickup or drop


9) At every one block of movement car will print all the information about its current position,
   current passengers in car, passengers picked up at current point and if there are more passengers
   to be picked up at other locations

10) If a passenger's end point is out of map the app will not store its info, the passengers end pt has to be within 
    the xMax, yMax grid 

11) The shortest routing alogrithm (basically a bfs) cares to drop a person first if it has to choose between a drop location
    or new passenger pickup a same distance from its current location
