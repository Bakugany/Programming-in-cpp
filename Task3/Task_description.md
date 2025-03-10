# Knights Tournament

## Introduction

In this assignment, students are expected to learn and practice:

- Different types and availability of constructors,
- Creating operators as class methods and global functions,
- Comparison operators,
- Explicit and implicit type conversions,
- Initializer lists,
- The use of the keywords `const`, `constexpr`, `consteval`, and `constinit`,
- The use of the inline keyword.

## Task

The task is to implement a module called `knights` that enables the simulation of a knights tournament for the royal cup (or the hand of a princess, impressively cast in stone). The module must expose the following classes:

- **Knight** – representing a single brave knight.
- **Tournament** – representing the knights tournament.

### Knight Class

Each knight should hold a certain number of gold coins for a hearty meal after the trials of battle and buying souvenirs in the tournament village. More importantly, a knight should be armed with a weapon and wear armor, each with a combat effectiveness represented by a class value. The higher the class of a weapon or armor, the stronger it is. A class value of zero indicates the absence of a weapon or armor. Gold, as well as the class values of weapons and armor, are represented by integers of type `size_t`.

The **Knight** class should provide:

- A constant `MAX_GOLD` representing the maximum number of gold coins a knight can possess; this corresponds to the maximum value of type `size_t`.
- Construction of a knight object **must require three parameters**: the number of gold coins, the weapon class, and the armor class. It should not be possible to create a knight without providing these parameters.
- The knight object should support default copy and move construction.
- Default copy and move assignment operations should be available.
- Methods to obtain information about the knight's gold, the class of the weapon they own, and the class of the armor they wear.
- A method to add a specified number of gold coins to the knight's sack, but only up to the allowed maximum.
- Methods to:
  - Force the knight to give away all his gold.
  - Change the knight's weapon to another of a specified class.
  - Discard his current weapon.
  - Change the knight's armor to another of a specified class.
  - Discard his current armor.
- The ability to take all the gold from another knight **and** only the better (i.e., higher class) pieces of his equipment. This operation should be implemented using the `operator +=`.
- The ability to create a new knight as the result of “adding” two knights (using the `operator +`). The new knight should have the total gold of the two knights (capped at `MAX_GOLD`) and the superior components of their equipment.
- Comparison of knights using the `<=>` operator and the `==` operator. The comparison answers the question: when can one knight defeat another? A duel is won primarily by the warrior whose weapon is stronger than the opponent's armor and whose armor class is at least as high as the opponent's weapon class. Furthermore, if both knights have weapons that are stronger than the opponent's armor, then the winner is the one with the better armor, or, if the armor classes are equal, the one with the better weapon. In all other cases, the duel is a draw, meaning the knights are considered equal.
- A way to print a knight’s information to standard output in the format:
  ```
  (gold, weapon class, armor class)
  ```

It is required that an object of the **Knight** class can be used in constant (`constexpr`) expressions. All methods that do not modify the state of a **Knight** should be available in constant expressions. Additionally, a global constant `TRAINEE_KNIGHT` must be defined and initialized at compile time; it represents a knight-in-training who has no gold and possesses a weapon and armor of class 1.

### Tournament Class

The **Tournament** class should allow for:

- Storing a list of contenders for the title and a list of knights eliminated from the tournament. The order in the tournament list determines the sequence in which knights will duel. The eliminated knights are arranged in the reverse order of their elimination.
- Creating a tournament object based on a list of knights passed as a parameter. If the provided list is empty, the only contender should be the `TRAINEE_KNIGHT`. It should not be possible to create a tournament without specifying a list of knights.
- Supporting copy and move construction, as well as copy and move assignment.
- Adding a knight to the end of the contenders list using the `operator +=`.
- Removing from the tournament all knights whose attributes (gold, weapon class, and armor class) are identical to those of a given knight. This should be implemented using the `operator -=`.
- Playing the tournament. The result of this operation should be a constant iterator to the container of contenders, pointing to the winner, or `end()` of the container if there is no winner. The tournament rules are as follows:
  - Always, the first two contenders in the list are selected to duel; they are then removed from the tournament list.
  - The duel is based on comparing the two knights as described in the **Knight** class comparison.
  - The winner takes all of the loser’s gold and the superior components of his equipment (weapon and armor).
  - The winner is then placed at the end of the contenders list.
  - The loser is placed in the eliminated knights list.
  - In case of a draw, both knights retain their gold and equipment, but both are moved to the eliminated knights list.
  - The tournament ends when the contenders list contains only one knight (the winner) or zero knights (no winner).
  - Knights moved to the eliminated list are inserted at the beginning of that list.
  - In the event of a draw, the order of the eliminated knights remains the same as it was in the contenders list.
- Providing a constant iterator that indicates the absence of a tournament winner.
- Obtaining information about the size of the tournament, i.e., the total number of participating knights (both contenders and eliminated knights).
- Printing to standard output the current list of contenders and eliminated knights. Each contender’s information should be preceded by the prefix "`+ `", and each eliminated knight’s information by "`- `". The end of the printing should be marked by a line containing the character "`=`".

At the beginning of each tournament play, the eliminated knights list should be cleared. The same clearing should take place whenever an operation changes the contents of the contenders list.

### Global Function

Finally, define a global function `max_diff_classes` that takes a list of knights and produces a result as a pair (`std::pair`) containing the weapon class and armor class whose difference is the greatest among the knights in the list. This function must be evaluable at compile time.

## Summary of Requirements

- **Knight Class:**
  - Constructor with parameters: gold, weapon class, armor class.
  - Constant `MAX_GOLD` and a compile-time constant `TRAINEE_KNIGHT`.
  - Methods for accessing and modifying gold, weapon, and armor.
  - Overloaded operators:
    - `operator +=` for taking gold and better equipment from another knight.
    - `operator +` for creating a new knight from two knights.
    - Comparison operators (`<=>`, `==`) to determine duel outcomes.
  - Support for `constexpr` usage.

- **Tournament Class:**
  - Maintains lists of contenders and eliminated knights.
  - Constructor that requires a list of knights (or defaults to `TRAINEE_KNIGHT` if empty).
  - Overloaded operators:
    - `operator +=` for adding a contender.
    - `operator -=` for removing knights matching given attributes.
  - Methods for playing the tournament, retrieving the tournament size, and printing the current state.
  - Clearing of the eliminated list upon modifications of the contenders list or at the start of tournament play.

- **Global Function:**
  - `max_diff_classes` must be computed at compile time.

This concludes the specification for the Knights Tournament module.
