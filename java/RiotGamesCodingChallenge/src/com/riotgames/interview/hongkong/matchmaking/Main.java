package com.riotgames.interview.hongkong.matchmaking;

import java.io.*;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Main {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		SampleData.initPlayerData();

		Matchmaker mm = new MatchmakerImpl(); 
		while ( Match.getTotalMatches() < 100 ) {
			Player p = SampleData.spawnPlayer();		
			long curTime = System.nanoTime();
			if (p != null) {
				int playersPerTeam = Math.random() > 0.6 ? 3 : 5;
				mm.enterMatchmaking(playersPerTeam, p);	
			}
			try {
				Thread.sleep(50);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}			
			mm.updateMatchList(curTime);
		}
		
		mm.terminateMatches();
		
		SampleData.writePlayerData();
		
		System.out.println("program terminated " + SampleData.getPlayers().size());  
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
    

}
