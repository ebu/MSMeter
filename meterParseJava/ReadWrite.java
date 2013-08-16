import java.io.IOException;
import java.math.BigInteger;


public class ReadWrite {

	public ReadWrite(int clientId){
		this.clientId = clientId;
		//initialising time here, check if good idea
		currentTime = new BigInteger("0");
	}
	
	
	private int clientId;
	private int blockSize;
	private String filename;
	private int mode;
	private double rate;
	private double delay;
	private int frameSize;
	private BigInteger currentTime;	//used to calculate time for every frame
	
	public void receiveLine(String line) throws IOException{
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
	// id,test000,0,3163,64,4.609234,5.864826
	private void lineProcessInit(String line){
		//System.out.println(clientId + " init : " + line);
		String[] csvs;
		csvs = line.substring(4).split(",");
		currentTime = new BigInteger("0"); //should be initialised with help of NTP
		filename = csvs[0];
		mode = Integer.parseInt(csvs[1]);
		blockSize = Integer.parseInt(csvs[3]);
		rate = Double.parseDouble(csvs[4]);
		delay = Double.parseDouble(csvs[5]);
		//System.out.println(filename + " - " + mode + " - " + blockSize);
	}
	
	//line containing NTP timestamp
	//  2,:t3567248571.582627
	private void lineProcessNTP (String line){
		//System.out.println(clientId + "  ntp : " + line);
		//if first time
		if (!parser.zeroTimeInit){
			parser.zeroTime = new BigInteger(line.substring(6));
			parser.zeroTimeInit = true;
		}
		//current time = (time from line) - (the zero time in parser's class)
		//System.out.println("DEBUG: " + line.substring(6) + " - " + parser.zeroTime);
		BigInteger ntpTime = new BigInteger(line.substring(6));
		//check if needed, not the best solution
		//trying to prevent the ntp lines that are too short...
		int digitsToAdd = 16 - line.substring(6).length();
		for (int i=0; i < digitsToAdd; i++){
			ntpTime = ntpTime.multiply(new BigInteger("10"));//adding zeros to ntpTime
		}
		
		currentTime = ntpTime.subtract(parser.zeroTime);
	} 
	
	//line containing frameSize
	//  2,:f20
		private void lineProcessFrameSize (String line){
			//System.out.println(clientId + "  ntp : " + line);
			frameSize = Integer.parseInt(line.substring(6));
		}
	
	//line containing arrays of delta times
	//  2,:43.366:18.838:17.616:18.539:15.894
	private void lineProcessTimes (String line) throws IOException{
		//System.out.println(clientId + " dtim : " + line);
		String[] deltaTimesString;
		deltaTimesString = line.substring(5).split(":");
		double[] deltaTimes = new double[deltaTimesString.length];
		for (int i = 0; i < deltaTimesString.length; i++){
			deltaTimes[i] = Double.parseDouble(deltaTimesString[i]);
			
			printDataToFile(deltaTimes[i]);
			
		}
	}
	
	//will prepare the line to print with the time etc...
	private void printDataToFile(double dTime) throws IOException{
		
		parser.output.write(clientId + "," + mode + "," + blockSize + "," + frameSize + "," + currentTime + "," + dTime);
		
		Integer dTimeInMicrosec = (int)(dTime*1000);
		BigInteger toAdd = BigInteger.valueOf(dTimeInMicrosec.intValue());
		currentTime = currentTime.add(toAdd);
	}
	
	//prints data if the read or write was not partitioned in frames
	private void printSingleLineData(){
		
	}

	
	//getters
	public int getClientId() {
		return clientId;
	}

	
}
