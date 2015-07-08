package edu.auburn.eng.csse.comp3710.kaw0026.midterm.kaw0026haiku;

import android.app.FragmentManager;
import android.app.Activity;
import android.app.Fragment;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.app.FragmentTransaction;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.RemoteViews;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;


public class MainActivity extends Activity {
    public final static String EXTRA_MESSAGE = "edu.auburn.eng.csse.comp3710.midterm.kaw0026haiku.MESSAGE";
    PlaceholderFragment startFrag;
    String wordType;
    String[] adjArray, nounArray, verbArray,otherArray;
    ArrayList<String> line1= new ArrayList<>();
    int line1_index;
    ArrayList<String> line2= new ArrayList<>();
    int line2_index;
    ArrayList<String> line3 = new ArrayList<>();
    int line3_index;
    Spinner spinner;
    Button addBut,removeBut,refreshBut,displayBut;
    TextView textView1,textView2,textView3;
    boolean disableAdd = false;
    int syl1 = 5;
    int syl2 = 7;
    int lineNum = 1;
    int sylsLeft = syl1;
    HashMap<String,Integer> sylMap = new HashMap<String,Integer>();


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        syllableMapper();
        setContentView(R.layout.main);

        if (savedInstanceState == null) {
            line1_index=0;
            line2_index=0;
            line3_index=0;
            startFrag = new PlaceholderFragment();
            getFragmentManager().beginTransaction()
                    .add(R.id.container, startFrag)
                    .commit();
        }

    }  //end onCreate
    public void onRestoreInstanceState(Bundle savedInstanceState){
        super.onRestoreInstanceState(savedInstanceState);
        final TextView textView1 = (TextView)findViewById(R.id.line1);
        final TextView textView2 = (TextView)findViewById(R.id.line2);
        final TextView textView3 = (TextView)findViewById(R.id.line3);
        String saved1 = savedInstanceState.getString("line1");
        String saved2 = savedInstanceState.getString("line2");
        String saved3 = savedInstanceState.getString("line3");
        textView1.setText(saved1);
        textView2.setText(saved2);
        textView3.setText(saved3);
        line1_index = savedInstanceState.getInt("arrIndex1");
        line2_index = savedInstanceState.getInt("arrIndex2");
        line3_index = savedInstanceState.getInt("arrIndex3");
//        line1 = new ArrayList<String>(line1_index);
//        line2 = new ArrayList<String>(line2_index);
//        line3 = new ArrayList<String>(line3_index);
        line1 = savedInstanceState.getStringArrayList("array1");
        line2 = savedInstanceState.getStringArrayList("array2");
        line3 = savedInstanceState.getStringArrayList("array3");



        lineNum = savedInstanceState.getInt("lineNum");
        sylsLeft= savedInstanceState.getInt("sylsLeft");
        wordType = savedInstanceState.getString("wordType");
        adjArray = savedInstanceState.getStringArray("adjArray");
        nounArray = savedInstanceState.getStringArray("nounArray");
        verbArray = savedInstanceState.getStringArray("verbArray");
        otherArray = savedInstanceState.getStringArray("otherArray");
        syllableMapper();
        spinner = (Spinner)findViewById(R.id.spinner);
        spinner.setVisibility(View.VISIBLE);
        removeBut = (Button)findViewById(R.id.deleteWord);
        removeBut.setVisibility(View.VISIBLE);
        addBut = (Button)findViewById(R.id.btn_add);
        addBut.setVisibility(View.VISIBLE);
        displayBut = (Button)findViewById(R.id.displayBut);
        displayBut.setVisibility(View.VISIBLE);
        refreshBut= (Button)findViewById(R.id.refreshBut);
        refreshBut.setVisibility(View.VISIBLE);
        refreshSpinner();
    }


    public void onResume(){
        super.onResume();
        //syllableMapper();
        printTextView();


    }

    protected void onSaveInstanceState(Bundle outState){
        TextView text1 = (TextView)findViewById(R.id.line1);
        TextView text2 = (TextView)findViewById(R.id.line2);
        TextView text3 = (TextView)findViewById(R.id.line3);
        CharSequence userText1 = text1.getText();
        CharSequence userText2 = text2.getText();
        CharSequence userText3 = text3.getText();
        outState.putCharSequence("line1",userText1);
        outState.putCharSequence("line2",userText2);
        outState.putCharSequence("line3",userText3);
        outState.putInt("lineNum",lineNum);
        outState.putInt("sylsLeft",sylsLeft);
        outState.putStringArrayList("array1",line1);
        outState.putStringArrayList("array2",line2);
        outState.putStringArrayList("array3",line3);
        outState.putInt("arrIndex1",line1_index);
        outState.putInt("arrIndex2",line2_index);
        outState.putInt("arrIndex3",line3_index);
        outState.putString("wordType",wordType);
        outState.putStringArray("adjArray",adjArray);
        outState.putStringArray("nounArray",nounArray);
        outState.putStringArray("verbArray",verbArray);
        outState.putStringArray("otherArray",otherArray);
        super.onSaveInstanceState(outState);


    }


    //RadioClick
    public void radioClick(View v){
        //Is Button Checked?
        boolean checked = ((RadioButton) v).isChecked();
        if(checked){
            spinner = (Spinner)findViewById(R.id.spinner);
            spinner.setVisibility(View.VISIBLE);
            addBut = (Button)findViewById(R.id.btn_add);
            addBut.setVisibility(View.VISIBLE);
            removeBut = (Button)findViewById(R.id.deleteWord);
           // removeBut.setVisibility(View.VISIBLE);
            refreshBut= (Button)findViewById(R.id.refreshBut);
            refreshBut.setVisibility(View.VISIBLE);
            displayBut = (Button)findViewById(R.id.displayBut);
            displayBut.setVisibility(View.VISIBLE);

        }
        //Check Which button is Checked
        switch(v.getId()){
            case R.id.adjective: {

                Log.i("radio: ", "adjective");
                adjArray = getResources().getStringArray(R.array.adjectives);
                wordType = "adjective";
                createSpinner(adjArray);
                break;
            }
            case R.id.noun:{

                Log.i("radio: ", "noun");
                nounArray = getResources().getStringArray(R.array.nouns);
                wordType = "noun";
                createSpinner(nounArray);
                break;
            }
            case R.id.verb:{
                Log.i("radio: ", "verb");
               verbArray = getResources().getStringArray(R.array.verbs);
                wordType = "verb";
                createSpinner(verbArray);
                break;
            }
            default: {
                Log.i("radio: ", "other");
                otherArray = getResources().getStringArray(R.array.other);
                wordType = "other";
                createSpinner(otherArray);
                break;
            }
        }

    } //end Radio Click

    public void createSpinner(String[] ar){

        if(sylsLeft != 0) {
            final String[] ar2 = ar; //to reset spinner @ the onclick
            spinner = (Spinner) findViewById(R.id.spinner);
            int indexSize = ar.length;
            String[] newArray = new String[indexSize];
            final int[] syllable = new int[indexSize];
            String word;
            int wordSyl;
            int newArrayCounter = 0;
            for (int i = 0; i < indexSize; i++) {
                word = ar[i];
                wordSyl = Integer.parseInt(String.valueOf(word.charAt(0)));
                if (wordSyl <= sylsLeft) {
                    newArray[newArrayCounter] = word.substring(1);
                    syllable[newArrayCounter] = wordSyl;
                    newArrayCounter++;
                }
            }
            ArrayAdapter adapter = new ArrayAdapter(this, android.R.layout.simple_spinner_item, newArray);
            adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
            spinner.setAdapter(adapter);
            spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
                @Override
                public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                    final int index = parent.getSelectedItemPosition();
                    final String spinItem = (String) parent.getItemAtPosition(index);
                    Log.i("spinner: ", "selected " + parent.getItemAtPosition(index));

                    //change text on button
                    addBut = (Button) findViewById(R.id.btn_add);
                    addBut.setText("ADD \n\'" + parent.getItemAtPosition(index) + "\'\n TO THE HAIKU");
                    addBut.setOnClickListener(new View.OnClickListener() {
                        @Override
                        public void onClick(View v) {
                            addWord(spinItem, syllable[index]);
                            createSpinner(ar2);
                        }
                    });
                }
                @Override
                public void onNothingSelected(AdapterView<?> parent) {
                }
            });
        }
    } //end CreateSpinner

    public void addWord(String str, int strSyl){
            Log.i("ADD WORD:", str + " " +sylMap.get(str) + " " +lineNum);
            removeBut.setVisibility(View.VISIBLE);
            if(sylsLeft != 0) {
                sylsLeft -= strSyl;
                switch (lineNum) {
                    case 1: {
                        line1.add(str);
                        line1_index++;
                        break;
                    }
                    case 2: {
                        line2.add(str);
                        line2_index++;
                        break;
                    }
                    case 3: {
                        line3.add(str);
                        line3_index++;
                        break;
                    }
                    default: {
                        break;
                    }
                }
                printTextView();
                Log.i("Syllables Left:", "" + sylsLeft);
            }
            if(sylsLeft == 0){
                nextLine();
            }
    } //end AddWord

    //Remove Last Word Added
    public void removeClick(View v){ //remove word
        String str;
        addBut.setVisibility(View.VISIBLE);
        spinner.setVisibility(View.VISIBLE);
        int addSyls;
        Log.i("DELETE: ","lineNum " + lineNum + " , Syslefy " + sylsLeft );
        if(lineNum == 2 && sylsLeft == 7){
            lineNum--;
            sylsLeft = 0;
        }else if(lineNum == 3 && sylsLeft == 5){
            lineNum--;
            sylsLeft = 0;
        }


        switch(lineNum){
            case 1:{
                str = line1.get(--line1_index);
                line1.remove(line1_index);
                break;
            }case 2:{
                str = line2.get(--line2_index);
                line2.remove(line2_index);
                break;
            }default:{
                str = line3.get(--line3_index);
                line3.remove(line3_index);
                break;
            }
        }
        addSyls = sylMap.get(str);
        sylsLeft += addSyls;
        if(lineNum==1 && sylsLeft == 5){
            removeBut.setVisibility(View.INVISIBLE);
        }
        Log.i("DELETE: ","lineNum " + lineNum + " , Syslefy " + sylsLeft );
        printTextView();
        refreshSpinner();

    }// end remove word

    public void printTextView(){
            textView1 = (TextView)findViewById(R.id.line1);
            textView2 = (TextView)findViewById(R.id.line2);
            textView3 = (TextView)findViewById(R.id.line3);
            String newLine1,newLine2,newLine3;
        switch(lineNum){
            case 1: {
                newLine1 = "1) ";
                for(String word:line1){
                    newLine1 += word + " ";
                }
                textView1.setText(newLine1);
                break;
            }
            case 2: {
                newLine2 = "2) ";
                for(String word:line2){
                    newLine2 += word + " ";
                }
                textView2.setText(newLine2);
                break;
            }
            case 3: {
                newLine3 = "3) ";
                for(String word:line3){
                    newLine3 += word + " ";
                }
                textView3.setText(newLine3);
                break;
            }
        }
    }

    public void displayHaiku(View v){
        Intent intent = new Intent(this,DisplayHaiku.class);
        String l1 = "";
        String l2 = "";
        String l3 = "";
        String display;
        for(String word:line1){
            l1 += word + " ";
        }

        for(String word:line2){
            l2 += word + " ";
        }

        for(String word:line3){
            l3 += word + " ";
        }
        display = l1 + "\n" + l2 +"\n" + l3 +"\n";
        intent.putExtra(EXTRA_MESSAGE,display);
        startActivity(intent);

    }

    public void nextLine(){
        lineNum++;
        if(lineNum == 2){
            sylsLeft = syl2;
        } else if(lineNum == 3){
            sylsLeft = syl1;
        } else { // line number > 3 no more lines
            addBut = (Button)findViewById(R.id.btn_add);
            addBut.setVisibility(View.INVISIBLE);
            spinner = (Spinner)findViewById(R.id.spinner);
            spinner.setVisibility(View.INVISIBLE);
            lineNum=3;
            disableAdd = true;
        }
        Log.i("log change Line:"," Line Num " +lineNum);
    }

    public void refreshSpinner(){
        if(wordType.equalsIgnoreCase("adjective")){
            createSpinner(adjArray);
        }else if(wordType.equalsIgnoreCase("noun")){
            createSpinner(nounArray);
        }else if (wordType.equalsIgnoreCase("verb")){
            createSpinner(verbArray);
        }else{
            createSpinner(otherArray);
        }
    }


    public void refresh(View v){
        lineNum = 3;
        line3.clear();
        printTextView();
        lineNum = 2;
        line2.clear();
        printTextView();
        lineNum = 1;
        line1.clear();
        printTextView();
        sylsLeft = syl1;

        spinner.setVisibility(View.INVISIBLE);
        addBut.setVisibility(View.INVISIBLE);
        printTextView();
        RadioGroup rG = (RadioGroup)findViewById(R.id.radioGroup);
        rG.clearCheck();
        removeBut.setVisibility(View.INVISIBLE);
        refreshBut.setVisibility(View.INVISIBLE);
        displayBut.setVisibility(View.INVISIBLE);
        //onCreate(null);
    }

    //Build HashMap Syllable Map
    //Keeps trak of word syllables for deleting purpose
    public void syllableMapper(){
        String[] ar1 = getResources().getStringArray(R.array.adjectives);
        String[] ar2 = getResources().getStringArray(R.array.nouns);
        String[] ar3 = getResources().getStringArray(R.array.verbs);
        String[] ar4 = getResources().getStringArray(R.array.other);
        int index1 = ar1.length;
        int index2 = ar2.length;
        int index3 = ar3.length;
        int index4 = ar4.length;
        String word;
        int wordSyl;
        for(int j = 0; j < index1 ; j++){
            word = ar1[j];
            wordSyl = Integer.parseInt(String.valueOf(word.charAt(0)));
            word = word.substring(1);
            sylMap.put(word,wordSyl);
        }
        for(int j = 0; j < index2 ; j++){
            word = ar2[j];
            wordSyl = Integer.parseInt(String.valueOf(word.charAt(0)));
            word = word.substring(1);
            sylMap.put(word,wordSyl);
        }
        for(int j = 0; j < index3 ; j++){
            word = ar3[j];
            wordSyl = Integer.parseInt(String.valueOf(word.charAt(0)));
            word = word.substring(1);
            sylMap.put(word,wordSyl);
        }
        for(int j = 0; j < index4 ; j++){
            word = ar4[j];
            wordSyl = Integer.parseInt(String.valueOf(word.charAt(0)));
            word = word.substring(1);
            sylMap.put(word,wordSyl);
        }
    }

    /**
     * A placeholder fragment containing a simple view.
     *
     */
    public static class PlaceholderFragment extends Fragment {
        String text1;
        String text2;
        String text3;
        public PlaceholderFragment() {
        }

        @Override
        public View onCreateView(LayoutInflater inflater, ViewGroup container,
                                 Bundle savedInstanceState) {
            View rootView = inflater.inflate(R.layout.fragment, container, false);


            return rootView;
        }
    }
} //End MainActivity
