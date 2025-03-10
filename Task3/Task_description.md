The task consists of implementing a module named "knights" that allows playing a knights’ tournament for either the King’s Cup or the Princess’s hand (imprinted, of course, in stone). Within this module, the following classes should be provided:

• Knight – representing a single valiant knight;

• Tournament – representing the knights’ tournament.

Each knight of class Knight should hold a certain number of gold coins for a proper meal after the hardships of battle and buying souvenirs in the tournament village. More importantly, he should wield a weapon and wear armor, the combat value of which is determined by a certain class. The higher the class of the weapon or armor, the stronger they are. A class value of zero indicates the absence of a weapon or armor. Both gold and the classes of weapon and armor are represented as integers of type size_t.

The Knight class should provide:

• A constant MAX_GOLD representing the maximum number of gold coins a knight can possess—that is, the maximum value of type size_t.

• Construction of an object (a knight) using three parameters (number of gold coins, weapon class, and armor class). It should not be possible to create an object without providing these parameters.

• Construction of an object by default copying and moving.

• Default copy and move assignment operations.

• Methods to retrieve the information about the knight’s gold, the class of the weapon he wields, and the class of the armor he wears.

• A function to add (throw in) a given number of gold coins to the knight’s bag, but only until the permissible maximum is reached.

• Functions to force the knight to give away all his gold, to change his weapon to another of a given class, to surrender his entire weapon, to change his armor to another of a given class, and to give away his armor.

• An operation to take all the other knight’s gold and only the superior (in terms of class) components of his equipment; this operation should be implemented via the operator +=.

• The creation of a new knight as the result of "adding" (operator +) two knights. The new knight should have as much gold as the sum of the gold coins of the two combined knights (but at most MAX_GOLD coins) and the better components of their equipment.

• Comparison of knights (operators <=> and ==), where the logic answers the question of whether one knight can defeat another. In a duel (comparison), the warrior whose weapon is stronger than his opponent’s armor and whose armor class is at the same time not less than his opponent’s weapon class wins. Furthermore, if both knights have weapons that are stronger than the opponent’s armor, the one with the better armor wins, or the one with the better weapon wins when the armors are of the same class. In all other cases there will be a draw, meaning the compared knights are equal.

• A function to print information about the knight to the standard output in the form “(gold coins, weapon class, armor class)”.

It should be ensured that an object of class Knight can be used in constant expressions (constexpr). Additionally, in constant expressions, all methods that do not change the state of the object should be available. Moreover, a global constant TRAINEE_KNIGHT must be defined, initialized only during compilation, representing a knight trainee who possesses no gold and has both a weapon and armor of class one.

In the second required class, Tournament, it should be possible to:

• Store a list of contestants vying for the title of tournament winner and a separate list of knights eliminated from the tournament. The order in the tournament list determines the sequence in which the knights will engage in combat, while the knights in the eliminated list are arranged in the reverse order of their elimination from the tournament.

• Create an object (tournament) based on a list of knights provided as a parameter. If the provided list is empty, the only contestant in the tournament should be the knight trainee. It should not be possible to create a tournament without providing any parameter.

• Create an object by copying and moving, as well as performing copy and move assignment operations.

• Add a knight to the end of the contestants list using the operator +=.

• Remove from the tournament all contestants whose parameters (gold, weapon class, and armor class) match exactly those of a given knight, using the operator -=.

• Run the tournament. The outcome of this operation should be a constant iterator to the contestants container, pointing to the winner, or to the container’s end (end()) in case there is no tournament winner. The tournament rules are as follows: - Two first contestants step forward to fight and are removed from the tournament list. - The fight consists of comparing these two contestants. - The winner takes all the gold of the loser and the superior components of his equipment (weapon and armor). - The winner is added to the end of the contestants list. - The loser is added to the eliminated list. - In the case of a draw, both knights retain their gold and equipment, but they are added to the eliminated list. - The tournament ends when the contestants list contains one (the winner) or zero (no winner) knights. - Knights entering the eliminated list are inserted at the beginning of that list. - In the event of a draw, the order of the knights in the eliminated list remains the same as their order in the contestants list.

• Obtain a constant iterator that signifies the absence of a tournament winner.

• Retrieve information about the size of the tournament, that is, the total number of knights participating (both contestants and eliminated knights).

• Print to the standard output the current list of contestants and the eliminated knights. Each contestant should be preceded by the prefix "+ ", and each eliminated knight by the prefix "- ". The end of the output should be marked by a line containing the "=" character.

At the beginning of each tournament run, the eliminated knights list must be cleared. The same clearing should also occur with every operation that alters the contents of the contestants list.

Finally, you should define a global function max_diff_classes that takes a list of knights and returns a pair (std::pair) containing the weapon and armor classes for which the difference is the greatest in that list. This function can be executed only at compile time.
