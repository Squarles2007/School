import java.io.*;
import java.util.*;
/**
 * Hangman Menu.
 * @author  Kullen Williams (kullen@auburn.edu)
 * @author  Dean Hendrix (dh@auburn.edu)
 * @version 2014-04-11
 *
 */
public class A6 {

   public static void main(String[] args) {
      Scanner scan = new Scanner(System.in);
      boolean again = true;
      Hangman game = new Hangman();
      game.setSecretWord("");
      System.out.print("Welcome to Hangman.\nThis is a very special hangman game."
            + "\nThis hangman may be cheating on you.\n"
            + "Are you ready to play??  [Y/N]: ");
      char ans = scan.next().charAt(0);
      while(again){
         switch (ans) {
            case 'y':{}
            case 'Y':{
               Random rand = new Random();
               int play = rand.nextInt((100 - 0) + 1);
               if(play < 55){
                  game.playClean(false);
               }else {
                  game.playDirty(false);
               }
               break;
            }
            case 'N':{}
            case 'n':{
               System.out.print("Well let me know if you change your mind.");
               again = false;
               break;
            }
            case 'C':{}
            case 'c':{
                  game.playDirty(true);
            }
         } //end switch ans
         
         if(again){
            System.out.print("Would you like to play again? [Y/N]"
               +"\nIf you would like to see how the game is cheating enter [C]: ");
            ans = scan.next().charAt(0);
            switch (ans) {
               case 'N':{}
               case 'n':{ 
               System.out.print("\nWell let me know if you change your mind.");
               again = false;
               }
            }
        }
      } //while loop
    } //end main
  }// end class