# Car-dealer-tracker2
An interactive computer program that keeps track of car dealers, cars, and a list of manufacturers. Prompt user for commands.





The program should essentially prompt the user to enter a one letter command, and process the command as follows:

a (add), which should then read in one of: c, which should then also read the next string as a 17 character VIN string (more on this later), followed by the (integer number of miles the car has been driven, a string (with no spaces in it) representing the name of a dealership, and finally an integer representing the current price of the car (in dollars). So, the following: a c JHM12345678901234 17900 Weber 1995 should add a car with a VIN of JHM12345678901234 that has been driven for 17900 miles to the Weber dealership’s inventory at a price of $1995.

m, which should then read the next 3 character string as an abbreviation for a manu-facturer, followed by a string representing the actual name of the manufacturer. So, the following:a m JHM Honda should add a manufacturer named Honda, with a code of JHM. Note that the 3 characters in this code correspond to the first three characters of a VIN.

d, which should then read the next string as dealer name, the following integer as a zipcode and then a 10 digit string as a phone number. a d Fusz 63122 3149665404 should add a dealer named Fusz in zip code 63122 with phone number(314)966-5404.

l (list), which should then read in one of: c, which should list all of the cars across all dealerships. Each car should have its VIN, mileage, dealer name, and price printed. d, which should list all of the dealers. Each dealer printed out should include its name,its zipcode, and its phone number.

f (find), which should also read in one of the following characters: m, followed by the name of a manufacturer. It should then list all cars made by that manufacturer. z, followed by a zipcode. It should list all cars available at dealers in that zip code.
 
d (delete), which should also read in one of the following characters: c, followed by a VIN. The corresponding car should be removed from the system.  For
example:
d c JHM12345678901234

d, followed by a dealer name.  For
example:
d d Fusz

s (summarize), which should simply print out one line for each manufacturer that includes the average price of the cars made by that manufacturer.

q (quit), which should stop the running program.
