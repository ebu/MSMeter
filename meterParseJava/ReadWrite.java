import java.io.IOException;
import java.math.BigInteger;


public class ReadWrite {

	public ReadWrite(int clientId){
		this.clientId = clientId;
		currentTime = BigInteger.valueOf(0);
	}
	
	
	private int clientId;
	private int blockSize;
	private String filename;
	private int mode;
	private int frameSize;
	private BigInteger currentTime;	//used to calculate time for every frame
	
	//lines received follow this pattern:
	// ' id,[,:][tf]data'
	//examples for each type in lineProcess* functions
	public void receiveLine(String line) throws Exception{
		if (line.charAt(4) != ':'){
			lineProcessInit(line);
		} else if (line.charAt(5) == 't'){
			lineProcessNTP(line);
		} else if (line.charAt(5) == 'f'){
			lineProcessFrameSize(line);
		} else {
			lineProcessTimes(line);
		}
	}
	
	//called when init line
	//  2,test000,0,3163,64,4.609234,5.864826
	private void lineProcessInit(String line){
		//System.out.println(clientId + " init : " + line);
		String[] csvs;
		csvs = line.substring(4).split(","); //removing the client id and the comma from string
		currentTime = new BigInteger("0"); //should be initialised with help of NTP
		filename = csvs[0];
		mode = Integer.parseInt(csvs[1]);
		blockSize = Integer.parseInt(csvs[3]);
		Double.parseDouble(csvs[4]);
		Double.parseDouble(csvs[5]);
	}
	
	//line containing NTP timestamp
	//  2,:t3567248571582627
	private void lineProcessNTP (String line) throws Exception{
		//if first time
		if (!Parser.zeroTimeInit){
			Parser.zeroTime = new BigInteger(line.substring(6));
			Parser.output.write("NTP time of start - " + Parser.zeroTime);
			Parser.zeroTimeInit = true;
		}
		//current time = (time from line) - (the zero time in parser's class)
		BigInteger ntpTime = new BigInteger(line.substring(6));
		//trying to prevent the ntp lines that are too short
		int digitsToAdd = 16 - line.substring(6).length(); //could throw an exception, the program would terminate
		for (int i=0; i < digitsToAdd; i++){
			ntpTime = ntpTime.multiply(new BigInteger("10"));//adding zeros to ntpTime
		}
		
		currentTime = ntpTime.subtract(Parser.zeroTime);
	} 
	
	//line containing frameSize
	//  2,:f20
	private void lineProcessFrameSize (String line){
		frameSize = Integer.parseInt(line.substring(6));
	}
	
	//line containing arrays of delta times
	//  2,:43.366:18.838:17.616:18.539:15.894
	private void lineProcessTimes (String line) throws IOException{
		String[] deltaTimesString;
		deltaTimesString = line.substring(5).split(":"); //removing the '  (id):' part of the line
		double[] deltaTimes = new double[deltaTimesString.length];
		for (int i = 0; i < deltaTimesString.length; i++){
			deltaTimes[i] = Double.parseDouble(deltaTimesString[i]);
			
			printDataToFile(deltaTimes[i]);
			
		}
	}
	
	//will prepare the line to print with the time etc...
	private void printDataToFile(double dTime) throws IOException{
		
		Parser.output.write(clientId + "," + filename + "," + mode + "," + blockSize + "," + frameSize + "," + currentTime + "," + dTime);
		
		Integer dTimeInMicrosec = (int)(dTime*1000);
		BigInteger toAdd = BigInteger.valueOf(dTimeInMicrosec.intValue());
		currentTime = currentTime.add(toAdd);
	}
	
	
	
	//getters
	public int getClientId() {
		return clientId;
	}

	
}
