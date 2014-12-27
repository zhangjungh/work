package com.riotgames.interview.hongkong.matchmaking;

import java.util.Iterator;
import java.util.LinkedList;

/**
 * The matchmaking implementation that you will write.
 */
public class MatchmakerImpl implements Matchmaker {
	public static long totalMatches = 0;
	private LinkedList<Match> matchList = new LinkedList<Match>();
	
    public Match findMatch(int playersPerTeam, Player player) {
        for (Match m : matchList) {
        	if (m.isMatchAvailable(playersPerTeam) && m.balancingEvaluate(player))
        	{
        		//System.out.printf("found match\n");
        		return m;
        	}
        }
        return null;
    }

    public void enterMatchmaking(int playersPerTeam, Player player) {
        Match m = findMatch(playersPerTeam, player);
        if (m == null)
        {
        	m = new Match(playersPerTeam);
        	matchList.add(m);
        	totalMatches += 1;
			//System.out.printf("new Match: players: %d total %d\n", playersPerTeam, totalMatches);
        }
        m.addPlayerToTeam(player);
    }
    
    public void updateMatchList(long curTime) {
    	Iterator<Match> iter = matchList.iterator();  
    	while ( iter.hasNext() ) {  
    	    Match m = iter.next();  
    	    if (m.isMatchEnded()) {
    	    	iter.remove();
    	    }
    	    else {
    	    	m.matchUpdate(curTime);
    	    }
    	}
    }
    
    public long getTotalMatches() {
    	return totalMatches;
    }

}
