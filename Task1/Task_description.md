# Medal Classification Ranking

## Introduction

The C++ Standard Library provides implementations for many data structures, such as `pair`, `tuple`, `array`, `vector`, `unordered_set`, `set`, `string`, `unordered_map`, `map`, `queue`, etc., as well as implementations of basic algorithms such as `sort`, `lower_bound`, `upper_bound`, `max_element`, and so on. The goal of the first coursework assignment is to practice using this library. Students should:

- Learn the basics of using the STL (Standard Template Library),
- Realize the importance of validating input data,
- Learn how to make appropriate programming decisions,
- Reinforce the necessity of thoroughly testing the program.

## Task

Write a program that determines the medal classification ranking for the countries participating in the Olympic Games. The program reads data from standard input. A correct input line has one of the following three formats:

1. **Medal Award Information**  
   This consists of the country’s name and the type of medal, separated by a single space.  
   - The country’s name contains only letters from the English alphabet, consists of at least two characters, and the first letter is uppercase. The name may include spaces within it.  
   - The type of medal is represented by a digit:
     - `1` for gold,
     - `2` for silver,
     - `3` for bronze.
     
     In addition, the digit `0` may be provided as the type of medal, which means that the country is added to the ranking without receiving any medal. A country can be added to the ranking multiple times.

2. **Medal Revocation Information**  
   This is a string that begins with a minus sign (`-`) followed by a string in a format analogous to the medal award information, with the difference that the type of medal being revoked is represented by a positive digit.

3. **Ranking Request**  
   This is a string that begins with an equals sign (`=`), followed by three integers in the range from 1 to 999999. These numbers represent the weights used for comparing the numbers of gold, silver, and bronze medals, respectively. The numbers are separated by a single space and have no leading zeros. If no medal-related information has been entered yet, nothing is printed in response to the ranking request.

Input lines are separated by newline characters. A newline character after the last line is optional. The valid input data does not contain any whitespace characters other than those mentioned above.

## Program Output

The program prints the medal classification ranking to standard output. Each country’s name is printed on a separate line, preceded by its ranking position. In case of a tie, the country names are printed in lexicographical order, and the same ranking position is assigned to each tied country. Every output line ends with a newline character.

## Error Handling

The program continuously checks whether the input data contain errors. The following cases are considered erroneous (including, for example, a medal revocation for a medal the country does not have):

- Any input line that does not conform to one of the three valid formats.
- Revoking a medal that the country does not possess.

For each erroneous line, the program prints the following diagnostic message to standard output:

```
ERROR L
```

where `L` represents the line number (starting with 1). The error message ends with a newline. The program ignores the content of erroneous lines.
