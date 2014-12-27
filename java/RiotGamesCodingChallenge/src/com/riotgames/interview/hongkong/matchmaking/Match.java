package com.riotgames.interview.hongkong.matchmaking;

import java.util.Set;

public class Match {
	private final static int K = 32;
	private final static int MATCH_DURATION = 1000;
    private final Set<Player> team1;
    private final Set<Player> team2;

    public Match(Set<Player> team1, Set<Player> team2) {
        this.team1 = team1;
        this.team2 = team2;
    }

    public Set<Player> getTeam1() {
        return team1;
    }

    public Set<Player> getTeam2() {
        return team2;
    }
    
    public double getTeam1Rank() {
    	if (team1.isEmpty())
    		return 0.0;
    	
    	double r = 0.0;
    	for (Player p : team1)
    		r += p.getRank();
    	return r / team1.size();
    }
    
    public double getTeam2Rank() {
    	if (team2.isEmpty())
    		return 0.0;
    	
    	double r = 0.0;
    	for (Player p : team2)
    		r += p.getRank();
    	return r / team2.size();    	
    }
    
    public void setMatchResult(int winTeam, double team1Rank, double team2Rank) {
    	if (winTeam == 1)
    	{
    		double dr = getDeltaRank(team1Rank, team2Rank);
    		for (Player p : team1)
    			p.setMatchResult(true, dr);
    		for (Player p : team2)
    			p.setMatchResult(false, -dr);
    	}
    	else if (winTeam == 2)
    	{
    		double dr = getDeltaRank(team2Rank, team1Rank);
    		for (Player p : team1)
    			p.setMatchResult(false, -dr);
    		for (Player p : team2)
    			p.setMatchResult(true, dr);
    	}
    }

	public static double getRate(double r1, double r2) {
		double e = (r2 - r1) / 400.0;
		double r = 1 + Math.pow(10, e);
		return 1.0 / r;
	}
	
	public static double getDeltaRank(double r1, double r2) {
		double rate = getRate(r1, r2);
		return (1 - rate) * K;
	}
	
	public void matchEnd() {
		try {
			Thread.sleep(MATCH_DURATION);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	double r1 = getTeam1Rank();
    	double r2 = getTeam2Rank();
		double r = Math.random();
		
		int winTeam;
		if (r > 0.7)
			winTeam = (r1 > r2) ? 2 : 1;
		else
			winTeam = (r1 > r2) ? 1 : 2;
		
		setMatchResult(winTeam, r1, r2);
	}
}
