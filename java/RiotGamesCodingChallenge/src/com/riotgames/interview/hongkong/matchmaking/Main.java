package com.riotgames.interview.hongkong.matchmaking;

import java.io.*;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Main {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		try {
			SampleData.initPlayerData();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		Matchmaker mm = new MatchmakerImpl(); 
		while ( mm.getTotalMatches() < 100 ) {
			Player p = SampleData.spawnPlayer();		
			long curTime = System.nanoTime();
			if (p != null) {
				int playersPerTeam = Math.random() > 0.6 ? 3 : 5;
				mm.enterMatchmaking(playersPerTeam, p);	
			}
			mm.updateMatchList(curTime);
			try {
				Thread.sleep(50);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
		try {
			parseConfigs();
			//writeFile();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		  
	}
	
    public static void parseConfigs() throws IOException {
        FileReader fr = new FileReader("configs.txt");
        BufferedReader br = new BufferedReader(fr);
        String line;
        while ((line=br.readLine()) != null) 
        {
            //arrs = line.split(",");
            System.out.println(line);
        }
        br.close();
        fr.close();
    }
    
    public static void writeFile() throws IOException {
        String[] arrs={
            "zhangsan,23,FuJian",
            "lisi,30,ShangHai",
            "wangwu,43,BeiJing",
            "laolin,21,ChongQing",
            "ximenqing,67,GuiZhou"
        };
        FileWriter fw = new FileWriter(new File("write.txt"));
        BufferedWriter bw = new BufferedWriter(fw);

        for (String arr:arrs)
        {
            bw.write(arr+"\t\n");
        }
        bw.close();
        fw.close();
    }   
}
