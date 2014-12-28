package com.riotgames.interview.hongkong.matchmaking;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class Config {
	private static int K = 32;
	private static double HIGHRATE = 0.55;
	private static double LOWRATE = 0.45;
	private static double TIMEOUTRATE = 0.05;
	private static long MATCH_DURATION = 1000 * 1000000L; //nanoseconds
	private static long MAX_WAIT_TIME =  2000 * 1000000L; //nanoseconds
	private static double INIT_RANK = 1400; //default Elo rank 
	private static double RANGE = 400; //rank adjustment range
	
    private static void readConfig(String filename) throws IOException {
        FileReader fr = new FileReader(filename);
        BufferedReader br = new BufferedReader(fr);
        String line;
        while ((line=br.readLine()) != null) 
        {
            String[] arr = line.split("=");
            if (arr.length == 2)
            {
            	if (arr[0].equals("highrate"))
            		HIGHRATE = new Double(arr[1].trim()); 
            	else if (arr[0].equals("lowrate"))
            		LOWRATE  = new Double(arr[1].trim());
            	else if (arr[0].equals("k"))
            		K = new Integer(arr[1].trim());
            	else if (arr[0].equals("timeoutrate"))
            		TIMEOUTRATE  = new Double(arr[1].trim());
            	else if (arr[0].equals("initrank"))
            		INIT_RANK = new Double(arr[1].trim());
            	else if (arr[0].equals("range"))
            		RANGE = new Double(arr[1].trim());
            	else if (arr[0].equals("matchduration"))
            		MATCH_DURATION = new Long(arr[1].trim()) * 1000000L;
            	else if (arr[0].equals("maxwaittime"))
            		MAX_WAIT_TIME = new Long(arr[1].trim()) * 1000000L;
            }
        }
        br.close();
        fr.close();
    }
    
    public static void initConfig() {
    	try {
			readConfig("configs.txt");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    }
    
    public static int getK() {
    	return K;
    }
    
    public static double getHighRate() {
    	return HIGHRATE;
    }
    
    public static double getLowRate() {
    	return LOWRATE;
    }
    
    public static double getTimeoutRate() {
    	return TIMEOUTRATE;
    }
    
    public static long getMatchDuration() {
    	return MATCH_DURATION;
    }
    
    public static long getMaxWaitTime() {
    	return MAX_WAIT_TIME;
    }
    
    public static double getInitRank() {
    	return INIT_RANK;
    }
    
    public static double getRange() {
    	return RANGE;
    }
}
