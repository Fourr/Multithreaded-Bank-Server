package search;

import java.io.*;
import java.util.*;

/**
 * This class encapsulates an occurrence of a keyword in a document. It stores the
 * document name, and the frequency of occurrence in that document. Occurrences are
 * associated with keywords in an index hash table.
 * 
 * @author Sesh Venugopal
 * 
 */
class Occurrence {
	/**
	 * Document in which a keyword occurs.
	 */
	String document;

	/**
	 * The frequency (number of times) the keyword occurs in the above document.
	 */
	int frequency;

	/**
	 * Initializes this occurrence with the given document,frequency pair.
	 * 
	 * @param doc Document name
	 * @param freq Frequency
	 */
	public Occurrence(String doc, int freq) {
		document = doc;
		frequency = freq;
	}

	/* (non-Javadoc)
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		return "(" + document + "," + frequency + ")";
	}
}

/**
 * This class builds an index of keywords. Each keyword maps to a set of documents in
 * which it occurs, with frequency of occurrence in each document. Once the index is built,
 * the documents can searched on for keywords.
 *
 */
public class LittleSearchEngine {

	/**
	 * This is a hash table of all keywords. The key is the actual keyword, and the associated value is
	 * an array list of all occurrences of the keyword in documents. The array list is maintained in descending
	 * order of occurrence frequencies.
	 */
	HashMap<String,ArrayList<Occurrence>> keywordsIndex;

	/**
	 * The hash table of all noise words - mapping is from word to itself.
	 */
	HashMap<String,String> noiseWords;

	/**
	 * Creates the keyWordsIndex and noiseWords hash tables.
	 */
	public LittleSearchEngine() {
		keywordsIndex = new HashMap<String,ArrayList<Occurrence>>(1000,2.0f);
		noiseWords = new HashMap<String,String>(100,2.0f);
	}

	/**
	 * This method indexes all keywords found in all the input documents. When this
	 * method is done, the keywordsIndex hash table will be filled with all keywords,
	 * each of which is associated with an array list of Occurrence objects, arranged
	 * in decreasing frequencies of occurrence.
	 * 
	 * @param docsFile Name of file that has a list of all the document file names, one name per line
	 * @param noiseWordsFile Name of file that has a list of noise words, one noise word per line
	 * @throws FileNotFoundException If there is a problem locating any of the input files on disk
	 */
	public void makeIndex(String docsFile, String noiseWordsFile) 
			throws FileNotFoundException {
		// load noise words to hash table
		Scanner sc = new Scanner(new File(noiseWordsFile));
		while (sc.hasNext()) {
			String word = sc.next();
			noiseWords.put(word,word);
		}

		// index all keywords
		sc = new Scanner(new File(docsFile));
		while (sc.hasNext()) {
			String docFile = sc.next();
			HashMap<String,Occurrence> kws = loadKeyWords(docFile);
			mergeKeyWords(kws);
		}


	}

	/**
	 * Scans a document, and loads all keywords found into a hash table of keyword occurrences
	 * in the document. Uses the getKeyWord method to separate keywords from other words.
	 * 
	 * @param docFile Name of the document file to be scanned and loaded
	 * @return Hash table of keywords in the given document, each associated with an Occurrence object
	 * @throws FileNotFoundException If the document file is not found on disk
	 */
	public HashMap<String,Occurrence> loadKeyWords(String docFile) 
			throws FileNotFoundException {
		HashMap<String,Occurrence> keyWords = new HashMap<String,Occurrence>();
		String fileName = docFile;
		Scanner sc = new Scanner(new File(docFile));

		while(sc.hasNext()){
			String word = sc.next();
			String newWord = getKeyWord(word);
			if(newWord != null){

				Occurrence o = keyWords.get(newWord);

				if(o ==null){
					o = new Occurrence(docFile, 1);
					keyWords.put(newWord, o);
				}else {
					o.frequency++;
				}
			}
		}
		return keyWords;
	}

	/**
	 * Merges the keywords for a single document into the master keywordsIndex
	 * hash table. For each keyword, its Occurrence in the current document
	 * must be inserted in the correct place (according to descending order of
	 * frequency) in the same keyword's Occurrence list in the master hash table. 
	 * This is done by calling the insertLastOccurrence method.
	 * 
	 * @param kws Keywords hash table for a document
	 */
	public void mergeKeyWords(HashMap<String,Occurrence> kws) {
		for (String key: kws.keySet()) {
			if(keywordsIndex.containsKey(key)==false){
				ArrayList<Occurrence> sinister = new ArrayList<Occurrence>();
				Occurrence BUST = kws.get(key);
				sinister.add(BUST);
				insertLastOccurrence(sinister);
				keywordsIndex.put(key, sinister );
			}
			else{
				ArrayList<Occurrence> four = keywordsIndex.get(key);
				Occurrence BUST2 = kws.get(key);
				four.add(BUST2);
				insertLastOccurrence(four);
				keywordsIndex.put(key, four );
			}
		}
	}

	/**
	 * Given a word, returns it as a keyword if it passes the keyword test,
	 * otherwise returns null. A keyword is any word that, after being stripped of any
	 * TRAILING punctuation, consists only of alphabetic letters, and is not
	 * a noise word. All words are treated in a case-INsensitive manner.
	 * 
	 * Punctuation characters are the following: '.', ',', '?', ':', ';' and '!'
	 * 
	 * @param word Candidate word
	 * @return Keyword (word without trailing punctuation, LOWER CASE)
	 */
	public String getKeyWord(String word) {
		if(word ==null){
			return null;
		}


		word = word.toLowerCase();
		if(word == ""){
			return null;
		}
		if(Character.isLetter(word.charAt(0)) == false){
			return null;
		}

		for(int i =0; i<word.length()-1; i++){
			if(Character.isDigit(word.charAt(i)) == true){
				return null;
			}
			if(Character.isLetter(word.charAt(i)) ==false){
				for(int j =i; j<word.length(); j++){
					if(Character.isLetter(word.charAt(j)) == true){
						return null;
					}
				}
			}
			if(Character.isLetter(word.charAt(i)) ==false &&Character.isLetter(word.charAt(i+1)) == false){

				String newWord = word.substring(0,i);
				if(noiseWords.containsKey(newWord) == true){
					return null;
				}
				else return newWord;

			}  
			if(((i+2) == word.length()) && Character.isLetter(word.charAt(i+1)) == false){
				String newWord = word.substring(0,i+1);
				if(noiseWords.containsKey(newWord) == true){
					return null;
				}
				else return newWord;

			}
		}
		if(noiseWords.containsKey(word) == true){
			return null;
		}

		else return word;

	}



	/**
	 * Inserts the last occurrence in the parameter list in the correct position in the
	 * same list, based on ordering occurrences on descending frequencies. The elements
	 * 0..n-2 in the list are already in the correct order. Insertion of the last element
	 * (the one at index n-1) is done by first finding the correct spot using binary search, 
	 * then inserting at that spot.
	 * 
	 * @param occs List of Occurrences
	 * @return Sequence of mid point indexes in the input list checked by the binary search process,
	 *         null if the size of the input list is 1. This returned array list is only used to test
	 *         your code - it is not used elsewhere in the program.
	 */
	public ArrayList<Integer> insertLastOccurrence(ArrayList<Occurrence> occs) {
		if(occs == null){
			return null;
		}		
		ArrayList<Integer> Reaper = new ArrayList<Integer>();
		if(occs.size() == 1){
			return Reaper; 
		}



		Occurrence Sinister = occs.get(occs.size()-1);
		occs.remove(occs.size()-1);
		int low = occs.size()-1;
		int high = 0;
		int mid = 0;
		int	freq = Sinister.frequency;
		while(high <= low) {
			mid = (low + high) / 2;
			Occurrence bust = occs.get(mid);
			int last = bust.frequency;
			if(last == freq) {
				Reaper.add(mid);
				break;
			}
			if(last < freq) {
				low = mid -1;
				Reaper.add(mid);
			}
			if(last > freq) {
				high = mid + 1;
				Reaper.add(mid);
				mid = mid+1;
			}

		}

		occs.add(mid, Sinister);


		return Reaper;}





	/**
	 * Search result for "kw1 or kw2". A document is in the result set if kw1 or kw2 occurs in that
	 * document. Result set is arranged in descending order of occurrence frequencies. (Note that a
	 * matching document will only appear once in the result.) Ties in frequency values are broken
	 * in favor of the first keyword. (That is, if kw1 is in doc1 with frequency f1, and kw2 is in doc2
	 * also with the same frequency f1, then doc1 will appear before doc2 in the result. 
	 * The result set is limited to 5 entries. If there are no matching documents, the result is null.
	 * 
	 * @param kw1 First keyword
	 * @param kw1 Second keyword
	 * @return List of NAMES of documents in which either kw1 or kw2 occurs, arranged in descending order of
	 *         frequencies. The result size is limited to 5 documents. If there are no matching documents,
	 *         the result is null.
	 */
	public ArrayList<String> top5search(String kw1, String kw2) {
		if(keywordsIndex.get(kw1) == null && keywordsIndex.get(kw2) ==null){
			return null;
		}
		ArrayList<Occurrence> sinister  = keywordsIndex.get(kw1);
		ArrayList<Occurrence> reaper =  keywordsIndex.get(kw2);
		ArrayList<String> megacow = new ArrayList<String>();
		if(keywordsIndex.containsKey(kw1) == false && keywordsIndex.containsKey(kw2) == false  ){
			return megacow;
		}
		else if (keywordsIndex.containsKey(kw2) == false ){
			for(int i =0; i<= sinister.size()-1; i++){
				megacow.add(sinister.get(i).document);
				
			}
			return megacow;
		}
		else if (keywordsIndex.containsKey(kw1) == false ){
			for(int i =0; i<= reaper.size()-1; i++){
				megacow.add(reaper.get(i).document);
				
			}
			return megacow;
		}
		
		int j =0;
		for(int i = 0; i<=4; i++)
		{
			int size = sinister.size()-1;
			int size2 = reaper.size()-1;
			if(size >= i && size2>= j){
				int freq=sinister.get(i).frequency;
				int freq2 = reaper.get(j).frequency;
				if(freq >= freq2){
					megacow.add(sinister.get(i).document);
				}
				if(freq2 < freq){
					megacow.add(reaper.get(j).document);
					j++;
				}
				else continue;
			}
			else if (size >= i){
				megacow.add(sinister.get(i).document);
				i++;
			}
			else if(size2 >= j){
				megacow.add(reaper.get(j).document);
				j++;
			}
			else break;

		}
		return megacow;
	}
}
