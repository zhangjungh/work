package com.riotgames.interview.hongkong.matchmaking;

import java.util.Set;
import java.util.HashSet;

public class Match {
	private final static int K = 32;
	private final static double HIGHRATE = 0.55;
	private final static double LOWRATE = 0.45;
	private final static long MATCH_DURATION = 1000 * 1000000L; //nanoseconds
	private static int MAX_MATCH_ID = 0;
    enum MatchStatus{ WAITING, START, END }
    private MatchStatus status;
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
    
	public void addPlayerToTeam(Player p) {
    	if (team1.size() <= team2.size())
    		team1.add(p);
    	else
    		team2.add(p);
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
    
    public boolean balancingEvaluate(Player p) {
    	double pr = p.getRank();
    	double r1 = getTeam1Rank();
    	double r2 = getTeam2Rank();
    	
    	double p1 = getRate(pr, r1);
    	double p2 = getRate(pr, r2);
    	
    	if ( (p1 > LOWRATE && p1 < HIGHRATE) || (p2 > LOWRATE && p2 < HIGHRATE) )
    		return true;
    	
    	return false;
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
    	double r1 = getTeam1Rank();
    	double r2 = getTeam2Rank();
		double r = Math.random();
		
		int winTeam;
		if (r > 0.7)
			winTeam = (r1 > r2) ? 2 : 1;
		else
			winTeam = (r1 > r2) ? 1 : 2;

		System.out.printf("Match %4d ended, team %2d win\n",  matchID, winTeam);
		
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
			System.out.printf("Match %4d force termianted\n",  matchID);
			
			SampleData.putPlayersToPool(team1);
			SampleData.putPlayersToPool(team2);
			
			status = MatchStatus.END;
    	}
	}
	
	public void matchStart(long st) {
		System.out.printf("Match %4d started, numbers %2d, win rate %f\n",  matchID, playersPerTeam*2, getRate(getTeam1Rank(), getTeam2Rank()));
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
			if (curTime - startTime >= MATCH_DURATION) {
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
