import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.math.BigInteger;
import java.util.LinkedList;


public class parser {
	
	static MeterOutput output;
	static BigInteger zeroTime;	//Ntp time at the beginning of each "session"
								//is used to substract from all subsequent NTP's
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
			} catch (IOException e) {
				System.out.println("IO Error: Parser will end");
				e.printStackTrace();
			}
			
		} else {
			System.out.println("Error: needs 2 arguments - inputfile outputfile");
		}
		
	}
	
	
	public static void readAndParse(String filename) throws IOException{
		
		LinkedList<ReadWrite> rwList = new LinkedList<ReadWrite>();
		
		
		BufferedReader br = new BufferedReader(new FileReader(filename));
		String line;
		while ((line = br.readLine()) != null) {
			
			if (line.charAt(0) == 'R'){
				break;//this is to stop reading the file for the Results Summary
			}
			
			int clientId = Integer.parseInt((line.substring(0, 3)).trim());
			
			//if a "stack"- instance exists, we add the line to it
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
			rwList.get(rwListId).receiveLine(line);
		}
		br.close();
		
	}
	

}
