package search;

import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

public class LSEDriver {

	public static void main(String[] args) throws FileNotFoundException {

		Scanner sc = new Scanner(System.in);

		LittleSearchEngine tester = new LittleSearchEngine();

		tester.makeIndex("docs.txt","noisewords.txt");

		while(true){
			System.out.print("Options: (1 to quit)(2 For Frequencies)(3 For Top 5 Searches): ");
			String key = sc.nextLine();

			System.out.println();
			if(key.equals("1")){break;}

			if(key.equals("2")){

				while(true){
					System.out.print("Enter key (1 to quit): ");
					key = sc.nextLine();
					if(key.equals("1")){break;}

					if(tester.keywordsIndex.containsKey(key) == false ){
						System.out.println("not found");
						continue;
					}



					ArrayList<Occurrence> list = tester.keywordsIndex.get(key);




					for(int i = 0; i < list.size(); i ++){ //this should be in decreasing order
						System.out.println("document file: " + list.get(i).document  + "| occurences:  " + list.get(i).frequency );

					}

				}
			}

			if(key.equals("3")){
				System.out.println("Enter a keyword (Enter 1 to quit)");
				String kw1 = sc.nextLine();

				//if(tester.keywordsIndex.containsKey(kw1) == false ){
					//System.out.println("not found");
					//continue;
				//}
				System.out.println("Enter another keyword");
				String kw2 = sc.nextLine();
				//if(tester.keywordsIndex.containsKey(kw2) == false ){
					//System.out.println("not found");
					//continue;
				//}


				ArrayList<String> bal =tester.top5search(kw1, kw2);

				for(int i = 0; i < bal.size(); i ++){ //this should be in decreasing order
					System.out.print(i + " document file: " + bal.get(i) + " ");
					System.out.println();
				}

			}


			System.out.println();

		}



	}
}

