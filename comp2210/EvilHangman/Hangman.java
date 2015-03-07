/**
*     HANGMAN
*/

import java.util.Scanner;
import java.util.TreeSet;
import java.util.Random;
import java.io.*;


/**
 * Plays both clean and dirty Hangman. With extensions,
 * could be used to hustle.
 *
 * Note: Some fields and methods, as well as the class
 * composition, reflect convenience for testing/grading
 * and not necessarily good game design.
 *
 * @author  Kullen Williams (kullen@auburn.edu)
 * @author  Dean Hendrix (dh@auburn.edu)
 * @version 2014-04-11
 *
 */
 
public class Hangman {

   //** number of guesses is randomly choosen
   Random rand = new Random();
   
   //** scanner used for taking input from System.in
   private Scanner stdin;
    
   //** secret word for this play
   String secretWord;
   boolean secretWordSet;
   
   //** lexicon stores the dictionary 
   private TreeSet<String> lexicon = new TreeSet<String>();
   
   /** ************************************************************************
   * Instantiates a new Hangman game with a default lexicon.
   *///************************************************************************
   public Hangman() {
      this(null);
   }  
   //**************************************************************************
   
   
   
   /***************************************************************************
   * Instantiates a new Hangman game using the provided lexicon,
   * attaches a scanner to System.in using the line separator
   * as the delimiter.
   *  @param in - inputStream
   *///************************************************************************
   public Hangman(InputStream in) {
   
      if (in == null) {   //** default lexicon
         in = Hangman.class.getResourceAsStream("lowerwords.txt");
      }
   
      try {
         Scanner s = new Scanner(new BufferedReader(new InputStreamReader(in)));
         while (s.hasNext()) {
            String word = s.next().toLowerCase();
            int wordLength = word.length();
            lexicon.add(word);
            s.nextLine();
         }
         
         stdin = new Scanner(System.in);
         stdin.useDelimiter(System.getProperty("line.separator").toString());
      
         secretWordSet = false;
         setSecretWord("");
      } //end try
      
      catch (Exception e) {
         System.err.println("Error reading from InputStream.");
         System.exit(1);
      }
   }
   //**************************************************************************
   
   
   
   
  /****************************************************************************
   * Sets the current secret word and notes that it has
   * been set.
   *  @param word - String
   ***************************************************************************/
   public void setSecretWord(String word) {
      
      if (!lexicon.contains(word)) {
         int size = lexicon.size();
         int ranVal = (int) (Math.random() * size + 1);
         int i = 1;
         for (String element : lexicon) {
            secretWord = element;
            i++;
            secretWordSet = true;
            if (i == ranVal) {
               break;
            }
         }
      }
      else {
         secretWord = word;
         secretWordSet = true;
      }
   } //************************************************************************
  
   

 /*****************************************************************************
  * Plays a clean game of Hangman, as described
  * in the assignment handout.
  * @param verbose - boolean
  ****************************************************************************/
   public void playClean(boolean verbose) {
      System.out.println("\n\n**** H A N G M A N ****");
      if (verbose) {
         System.out.println("(playing clean)");
      }
      
      int wordLength = readWordLength();
      
      setSecretWord(secretWord);
      int numGuess = readNumGuesses();
      int guessRemain = numGuess;
      int wordsLeft = lexicon.size(); //Shows the num of possible choices left
      
      String wrongGuess = " ";
      String progress = secretWord;
      String alreadyGuessed = wrongGuess + secretWord;
     
      for (int i = 0; i < secretWord.length(); i++) { //builds masked word
         progress = progress.substring(0, i) + '-' + progress.substring(i + 1);
      }
       
      while (guessRemain > 0) {
         System.out.print("\n");
         if (verbose) {
            System.out.println("The secret word is:       " + secretWord);
         }
         System.out.println("Your progress:            " + 
            "/" + progress + "/");
         System.out.println("Wrong guesses:           " + wrongGuess);
         System.out.println("Guesses remaining:        " + guessRemain);
            
         if (verbose) {
            System.out.println("Number of words possible: " + wordsLeft);
         }
            
         char guessLetter = readLetterGuess();  //*** GUESS CHARACTER
             
         // loops through and makes sure the letter hasn't already been guessed
         alreadyGuessed = wrongGuess + progress;
         for (int j = 0; j < alreadyGuessed.length(); j++) {
            if (alreadyGuessed.charAt(j) == guessLetter) {
               System.out.println("You've already guessed this letter. "
                  + "Please pick another one!");
               j = 0;
               guessLetter = readLetterGuess();
            }
         }
         
         progress = progress(progress, guessLetter);
         wrongGuess = wrongGuess(wrongGuess, guessLetter);
            
         wordsLeft = wordsLeft(progress, guessLetter);
            
         if (progress.equals(secretWord)) {
            System.out.print("\nYou won! The secret word was " + progress);
            break;
         }
         
         guessRemain = guessRemaining(guessLetter, guessRemain);
         
      } // loop through number of guesses
      
      youLose(progress);
      
   } 
   //******PLAY CLEAN**********************************************************
   
   
 
   /***************************************************************************
   *  DIRTY
   * Plays a dirty game of Hangman, as described
   * in the assignment handout.
   * @param verbose - boolean
   ***************************************************************************/
   public void playDirty(boolean verbose) {
      System.out.println("\n\n**** HANGMAN ****");
      if (verbose) {
         System.out.println("(playing dirty)");
      }
      
      int wordLength = readWordLength();  //** SET WORD LENGTH
      setSecretWord(secretWord);          //** SET SECRET_WORD
      int numGuess = readNumGuesses();    //** SET NUM GUESS
      
      int guessRemain = numGuess;      //to start guesesRemaining = numGuess
      int wordsLeft = lexicon.size(); // Shows the num of possible coices left
      
      String wrongGuess = " ";
      String progress = secretWord;
            
      for (int i = 0; i < secretWord.length(); i++) {
         progress = progress.substring(0, i) + '-' + progress.substring(i + 1);
      }
       
      while (guessRemain > 0) {
         System.out.print("\n");
         if (verbose) {
            System.out.println("The secret word is:       " + secretWord);
         }
              
         System.out.println("Your progress:            " 
            + "/" + progress + "/");
         
         System.out.println("Wrong guesses:           " + wrongGuess);
         
         System.out.println("Guesses remaining:        " + guessRemain);
            
         if (verbose) {
            System.out.println("Number of words possible: " + wordsLeft);
         }
         
         
         char guessLetter = readLetterGuess();  //** GUESS LETTER
            
         //loops through all guesses 
         //makes sure letter hasn't already been guessed
         String alreadyGuessed = wrongGuess + progress;
         for (int j = 0; j < alreadyGuessed.length(); j++) {
            if (alreadyGuessed.charAt(j) == guessLetter) {
               System.out.println("You've already guessed this letter. "
                  + "Please pick another one!");
               j = 0;
               guessLetter = readLetterGuess();
            }
         }
            
       
         String families = dirtyDisplay(guessLetter, progress);
         if (verbose) {
            System.out.println("Possible word families: \n" + families);
         }
         
         
         progress = progress(progress, guessLetter);
         wrongGuess = wrongGuess(wrongGuess, guessLetter);
         wordsLeft = wordsLeft(progress, guessLetter);
        
         //**  If Secret Word is Found  ***
         if (progress.equals(secretWord)) {
            System.out.print("\nYou won! The secret word was " + progress);
            break;
         } 
         
         guessRemain = guessRemaining(guessLetter, guessRemain);
           
      } // end loop through number of guesses

      youLose(progress);
   
   } 
   // END PLAYING DIRTY *******************************************************



//*****************************************************************************
//                      - - -  G U T S - - - 
//*****************************************************************************

   /** ************************************************************************
   * Reads from System.in the word length for the game.
   * Only accepts valid lengths for the current lexicon.
   *///************************************************************************
   private int readWordLength() {
      int len = 0;
      
      TreeSet<String> tempLex = new TreeSet<String>();
      tempLex.addAll(lexicon);

            len = rand.nextInt((10 - 4) + 1) + 4;
           
            for (String word : tempLex) {
               if (word.length() != len) {
                  lexicon.remove(word);
               }
            }

      return len;
   }
   //**************************************************************************

   /*****************************************************************************
   * Reads from System.in the maximum number of guesses.
   *
   * @return numGuesses - int
   ****************************************************************************/
   private int readNumGuesses() {
      int numGuesses = 0;
      numGuesses = rand.nextInt((12-5)+1) + 5;
      return numGuesses;
   }
   //**************************************************************************

 
 
   /***************************************************************************
   * Reads from System.in a single letter guess.   -GUESS LETTER
   *
   *   @return ch - char
   *
   ***************************************************************************/
   private char readLetterGuess() {
      int ch = 0;
      boolean validChar = false;
      do {
         System.out.print("Guess a letter: ");
         ch = stdin.next().charAt(0);
         System.out.println("");  // MAKE SURE TO REMOVE AFTER TESTING
         stdin.nextLine(); 
      // ch needs validating
         if (ch >= 'a' && ch <= 'z') {
            validChar = true;
         }
         if (!validChar) {
            System.out.println(
               "You must enter a lowercase letter in the range ['a'..'z'].");
         }
      } while (!validChar);
      return (char) ch;
   }
   //**************************************************************************
   
   
   
   
   /***************************************************************************
   * Progress String
   * Unmasks the progress string every time a letter is guessed correctly.
   *
   * @param progressIn - String
   * @param guess - char
   * @return progress - String
   ***************************************************************************/
   private String progress(String progressIn, char guess) {
      String progress = progressIn;
   
      int i = 0;
   
      while (i < secretWord.length()) {
         if (secretWord.charAt(i) == guess) {
            progress = progress.substring(0, i) + guess 
               + progress.substring(i + 1);
         }
         i++;
      }
   
      return progress;
   }  
   
    /**************************************************************************
   * Wrong Guess
   * Keeps track of the chars that were guessed wrong and then prints
   * them together.
   *
   * @param wrongGuessIn - String
   * @param guess  - char
   * @return wrongGuess - String
   ***************************************************************************/
   private String wrongGuess(String wrongGuessIn, char guess) {
      String wrongGuess = wrongGuessIn;
      int i = 0;
      boolean found = false;
      while (i < secretWord.length()) {
         if (secretWord.charAt(i) == guess) {
            found = true;
         }
         i++;
      }
      if (!found) {
         wrongGuess = wrongGuess + guess;
      }
   
      return wrongGuess;
   }
   

   /***************************************************************************
   * wordsLeft
   * displays the number of possible words left.
   *
   * @param progressIn - String
   * @param guessIn - char
   * @return wordsLeft - int
   ***************************************************************************/
   private int wordsLeft(String progressIn, char guessIn) {
      String progress = progressIn;
      char guess = guessIn;
      TreeSet<String> tempLex = new TreeSet<String>();
      tempLex.addAll(lexicon);
   
      for (int i = 0; i < secretWord.length(); i++) {
         if (progress.charAt(i) == guess) {
            for (String word : tempLex) {
               if (word.charAt(i) != guess) {
                  lexicon.remove(word);
               }
            }
         }
      }
      return lexicon.size();
   } 
 
 
   /***************************************************************************
   * Dirty Display
   * displays the possible word families update lexicon to largest family
   * also changes secret word if necessary.
   *
   * @param guessIn - char
   * @param progressIn - String
   * @return dirtyDisplay - String
   ***************************************************************************/
   private String dirtyDisplay(char guessIn, String progressIn) {
      String progress = progressIn;
      char guess = guessIn;
      
      String key = null;
      String family = " ";
      String tempFamily = family;
   
      TreeSet<String> keySet = new TreeSet<String>();
      TreeSet<String> noMatch = new TreeSet<String>(lexicon);
      TreeSet<String> testLex = new TreeSet<String>();
      TreeSet<String> temp2 = new TreeSet<String>();
      
      
      // FIND KEYS for Word Families
      for (String word : lexicon) { //*******************************
         int j = 0;
         key = progress;  // key starts as progress
         
         while (j < secretWord.length()) {
            if (word.charAt(j) == guess) {
               key = key.substring(0, j) + guess + key.substring(j + 1);
            }
            j++;
         }
         
         if (!keySet.contains(key)) {
            keySet.add(key);
         }          
      } 
      // -- END Find KEYS
   
      
      // * * Find Family Groups
      for (String key1 : keySet) {     //** loop through keySet  **//
      
         if (!key1.equals(progress)) {  //** skip key == process   **//
         
            //** loop through set of nomatches
            for (String word1 : noMatch) { 
            
               /**
               *   the set noMatch is the set of word that do not contain the
               *   guessed leter.
               *   this should reduce for every key word that is used.
               */
            
               //** loop through every letter of word1 **//
               for (int i = 0; i < key1.length(); i++) { //********************
                                 
                  if (word1.charAt(i) == guess) {
                  
                     //not all guess vals match key and word1 remove from temp2
                     if (word1.charAt(i) != key1.charAt(i)) {   
                        temp2.remove(word1);
                        break;
                     }
                     
                     // add word to temp2
                     if (word1.charAt(i) == guess 
                        && word1.charAt(i) == key1.charAt(i)  
                        && !testLex.contains(word1)) {
                        temp2.add(word1);
                     }
                     // second check to remove from temp2
                     else if (word1.charAt(i) != key.charAt(i)   
                        && testLex.contains(word1)) {
                        temp2.remove(word1);
                     }
                  } //end if
                  
                  tempFamily = "\n" +  "/" + key1 + "/" + " " + temp2.size();
               } //************************************************************
               
            } //** END for (String key1 : keySet)
         
            //remove used values from noMatch set
            for (String word2 : temp2) {  
               noMatch.remove(word2); 
            }

            //Largest temp2 is stored in TestLex
            if (temp2.size() >= testLex.size()) { 
               testLex.clear();
               testLex.addAll(temp2);
            }
            tempFamily = "\n" + "/" + key1 + "/" + " " + temp2.size();   
         }
         
         family = family + tempFamily; //combine family and tempFamily
               
         temp2.clear();  //clear temp2 and start over
         
      } 
      //end for (key1: keyset)*************************************************
      
      if (noMatch.size() >= testLex.size()) {  // noMatch is new lexicon
         lexicon.clear();
         lexicon.addAll(noMatch);
      }
      else { //testLex is new lexicon
         lexicon.clear();
         lexicon.addAll(testLex);
      }
      
      
      setSecretWord(secretWord); //set new secretWord  
      
      
      family = "/" + progress + "/" + " " + noMatch.size() + family;
      
      return family;
      
      
   } 
   //END Dirty Display*********************************************************
   
   /***************************************************************************
   *  YOU LOSE
   *  If you are out of guess and did not guess the correct secretWord
   *  youLose will print message.
   *  @param progress - String
   ***************************************************************************/
   private void youLose(String progress) {
   
      if (!progress.equals(secretWord)) {
         System.out.println("Sorry, but you didn't guess the secret word: " 
            + secretWord + ".");
      }
   } 
   //**************************************************************************
   
   

   /***************************************************************************
   * GUESS REMAINING -
   * if player guesses a correct word the guess count doesn't decrement.
   *
   *  @param guessIn - char
   *  @param guessRemainIn - int
   *  @return - guessRemainIn - int
   */ //***********************************************************************
   private int guessRemaining(char guessIn, int guessRemainIn) {
      boolean correctGuess = false;
      for (int j = 0; j < secretWord.length(); j++) {
         if (secretWord.charAt(j) == guessIn) {
            correctGuess = true;
            break;
         } else {
            correctGuess = false;
         }
      }
      if (!correctGuess) { guessRemainIn--; }
   
      return guessRemainIn;
   } 
   //**************************************************************************
} 
// end HANGMAN
