Write a program that determines the medal ranking of the countries participating in the Olympic Games. The program reads data from standard input. A valid input line has one of the following three formats:
- Information about a country winning a medal,
- Information about a medal being taken away from a country,
- A request to print the current medal ranking.
The information about a country winning a medal consists of the country's name and the type of medal separated by a single space. The country's name contains only letters from the English alphabet, consists of at least two letters, the first letter is uppercase, and spaces may appear within the name. The type of medal is represented by the digit 1 (gold), 2 (silver), or 3 (bronze). The digit 0 may also be provided as the type of medal, which only means that the country is added to the ranking without a medal. A country can be added to the ranking multiple times.

The information about a medal being taken away from a country is a string consisting of a minus sign (-) followed by a string in a format analogous to that used for the information about winning a medal, with the difference that the type of the removed medal is a positive digit.

The request to print the current ranking is a string consisting of an equals sign (=) followed by three integers from the range 1 to 999999. These numbers denote the weights by which the numbers of gold, silver, and bronze medals are compared, respectively. The numbers are separated by a single space. The numbers do not have leading zeros. If no medal-related information has been entered yet, nothing is printed.

The input lines are separated by newline characters. After the last line, a newline character is optional. The valid input data does not contain any white space characters other than those mentioned above.

The program prints the medal ranking to standard output. The name of each country is printed on a separate line, preceded by its ranking position. In the case of a tie, the names of the countries are printed in lexicographical order, with the same ranking position given to each. Each line ends with a newline character.
The program continuously checks whether the input data contain errors. Also regarded as erroneous is any information about revoking a medal that the country does not have. For each erroneous line, the program prints on the standard error output the message

ERROR L

where L represents the line number (with line numbering starting at 1). The error message ends with a newline. The program ignores the contents of erroneous lines.
