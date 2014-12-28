package com.riotgames.interview.hongkong.matchmaking;

import java.util.Set;
import java.util.HashSet;

public class Match {
	private static int MAX_MATCH_ID = 0;
    enum MatchStatus{ WAITING, START, END }
    private MatchStatus status;
    private long waitTime = 0;
    private long startTime = 0;
    private Set<Player> team1;
    private Set<Player> team2;
    private int playersPerTeam;
    private int matchID;

    public Match( int playersPerTeam ) {
        this.team1 = new HashSet<Player>();
        this.team2 = new HashSet<Player>();
    	this.status = MatchStatus.WAITING;
    	this.playersPerTeam = playersPerTeam;
    	MAX_MATCH_ID += 1;
    	this.matchID = MAX_MATCH_ID;
    }
    
    public static int getTotalMatches() {
    	return MAX_MATCH_ID;
    }

    public Set<Player> getTeam1() {
        return team1;
    }

    public Set<Player> getTeam2() {
        return team2;
    }
    
	public void addPlayerToTeam(Player p, long curTime) {
    	if (team1.size() <= team2.size())
    		team1.add(p);
    	else
    		team2.add(p);
    	waitTime = curTime;
    }
    
    private double getTeam1Rank() {
    	if (team1.isEmpty())
    		return Config.getInitRank();
    	
    	double r = 0.0;
    	for (Player p : team1)
    		r += p.getRank();
    	return r / team1.size();
    }
    
    private double getTeam2Rank() {
    	if (team2.isEmpty())
    		return Config.getInitRank();
    	
    	double r = 0.0;
    	for (Player p : team2)
    		r += p.getRank();
    	return r / team2.size();    	
    }
    
    public boolean balancingEvaluate(Player p) {
    	double pr = p.getRank();
    	double r1 = getTeam1Rank();
    	double r2 = getTeam2Rank();
    	
    	double p1 = getRate(pr, r1);
    	double p2 = getRate(pr, r2);
    	
    	if ( (p1 > Config.getLowRate() && p1 < Config.getHighRate()) || (p2 > Config.getLowRate() && p2 < Config.getHighRate()) )
    		return true;
    	
    	return false;
    }
    
    public boolean isMatchWaitTooLong(long curTime) {
    	return (curTime - waitTime) > Config.getMaxWaitTime();
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

    // Elo formula to get the win rate with known rank
    // 1 / (1 + pow(10, (rb-ra) / 400))
	private static double getRate(double r1, double r2) {
		double e = (r2 - r1) / 400.0;
		double r = 1 + Math.pow(10, e);
		return 1.0 / r;
	}
	
	// Elo formula to add delta rank after a match (only consider win and lose)
	private static double getDeltaRank(double r1, double r2) {
		double rate = getRate(r1, r2);
		return (1 - rate) * Config.getK();
	}
	
	public void matchEnd() {
    	double r1 = getTeam1Rank();
    	double r2 = getTeam2Rank();
		double r = Math.random();
		
		int winTeam;
		if (r > 0.7)
			winTeam = (r1 > r2) ? 2 : 1;
		else
			winTeam = (r1 > r2) ? 1 : 2;

		System.out.printf("Match %3d ended, team %1d win\n",  matchID, winTeam);
		
		setMatchResult(winTeam, r1, r2);
		
		SampleData.putPlayersToPool(team1);
		SampleData.putPlayersToPool(team2);
		
		status = MatchStatus.END;
	}
	
	public void matchTerminate() {
    	if (team1.size() + team2.size() >= (playersPerTeam * 2))
    	{
    		matchEnd();
    	}
    	else
    	{
			System.out.printf("Match %3d force termianted\n",  matchID);
			
			SampleData.putPlayersToPool(team1);
			SampleData.putPlayersToPool(team2);
			
			status = MatchStatus.END;
    	}
	}
	
	public void matchStart(long st) {
		System.out.printf("Match %3d started, players %2d, win rate %f\n",  matchID, playersPerTeam*2, getRate(getTeam1Rank(), getTeam2Rank()));
		status = MatchStatus.START;
		startTime = st;		
	}
	
	public void matchUpdate(long curTime) {
		if (status == MatchStatus.WAITING) {
	    	if (team1.size() + team2.size() >= (playersPerTeam * 2)) {
	    		matchStart(curTime);
	    	}
		}
		else if (status == MatchStatus.START) {
			if (curTime - startTime >= Config.getMatchDuration()) {
				matchEnd();
			}			
		}		
	}
	
	public boolean isMatchAvailable(int players) {
		return (status == MatchStatus.WAITING && playersPerTeam == players);
	}
	
	public boolean isMatchEnded() {
		return status == MatchStatus.END;
	}
}
