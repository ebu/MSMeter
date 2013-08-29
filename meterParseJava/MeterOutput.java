import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;


public class MeterOutput {

	
	private BufferedWriter bw;
	private FileWriter fw;
	
	public MeterOutput(String filename) throws IOException {
		fw = new FileWriter(filename);
		bw = new BufferedWriter (fw);
	}
	
	public void write(String line) throws IOException{
		bw.write(line);
		bw.newLine();
		
	}
	
	public void close(){
		try {
			bw.close();
			fw.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
