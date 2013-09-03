import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.math.BigInteger;
import java.util.LinkedList;


public class Parser {
	
	static MeterOutput output;
	static BigInteger zeroTime;	//Ntp time at the beginning of each "session"
	static boolean zeroTimeInit = false; //zeroTime has to be initialised only once

	public static void main(String[] args) {		
		
		if (args.length >= 2){
		
			try {
				String csvfileName = args[0];
				output = new MeterOutput(args[1]);
				output.write("CLIENTID,FILENAME,MODE,BLOCKSIZE,BLOCKSINFRAME,TIME,DTIME");
				readAndParse(csvfileName);
				output.close();
				System.out.println("done.");
			} catch (Exception e) {
				System.err.println("IO Error: Parser will end");
				e.printStackTrace();
			}
		} else {
			System.err.println("Error: needs 2 arguments - inputfile outputfile");
		}
		
	}
	
	
	//reads a line, decides if useful or not, parses the ID, and "gives" it to the targeted instance
	public static void readAndParse(String filename) throws Exception{
		
		//TODO: replace by HashMap
		LinkedList<ReadWrite> rwList = new LinkedList<ReadWrite>();
		
		
		BufferedReader br = new BufferedReader(new FileReader(filename));
		String line;
		
		//a line in the file looks like this:
		//'  (id)(separator)(data............)'
		//if a line starts by R, we do not process it
		while ((line = br.readLine()) != null) {
			
			if (line.charAt(0) == 'R'){
				break;
			}
			
			int clientId = Integer.parseInt((line.substring(0, 3)).trim());
			
			//if a "stack"- instance of a certain client ID exists, we add the line to it
			boolean found = false;
			int rwListId = -1;
			for (int i = 0; i < rwList.size(); i++){
				if (clientId == rwList.get(i).getClientId()){
					found = true;
					rwListId = i;
				}
			}
			//or we create a new one
			if (!found){
				ReadWrite temp = new ReadWrite(clientId);
				rwListId = 0;
				rwList.add(rwListId, temp);
			}
			//we finally "give" the line to the correct instance
			//the clients instance will process the line to extract info
			rwList.get(rwListId).receiveLine(line);
		}
		br.close();//closing the wrapper closes the underlying FileReader
		
	}
	

}
