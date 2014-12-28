package com.riotgames.interview.hongkong.matchmaking;

public class Main {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Config.initConfig();
		SampleData.initPlayerData();

		Matchmaker mm = new MatchmakerImpl(); 
		while ( Match.getTotalMatches() < 100 ) {
			Player p = SampleData.spawnPlayer();		
			long curTime = System.nanoTime();
			if (p != null) {
				int playersPerTeam = Math.random() > 0.6 ? 3 : 5;
				mm.enterMatchmaking(playersPerTeam, p, curTime);	
			}
			try {
				Thread.sleep(20);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}			
			mm.updateMatchList(curTime);
		}
		
		mm.terminateMatches();	
		SampleData.writePlayerData();
		
		System.out.println("matchmaking finished!");  
	} 

}
