package com.riotgames.interview.hongkong.matchmaking;

import java.util.Iterator;
import java.util.LinkedList;

/**
 * The matchmaking implementation that you will write.
 */
public class MatchmakerImpl implements Matchmaker {
	private LinkedList<Match> matchList = new LinkedList<Match>();
	
    public Match findMatch(int playersPerTeam, Player player, long curTime) {
    	for (Match m : matchList) {
			if (m.isMatchAvailable(playersPerTeam) && m.balancingEvaluate(player)) {
        		return m;
        	}
        }        
    	for (Match m : matchList) {
        	if (m.isMatchAvailable(playersPerTeam) && m.isMatchWaitTooLong(curTime)) {
        		return m;
        	}
        }
        return null;
    }

    public void enterMatchmaking(int playersPerTeam, Player player, long curTime) {
        Match m = findMatch(playersPerTeam, player, curTime);
        if (m == null) {
        	m = new Match(playersPerTeam);
        	matchList.add(m);
        }
		m.addPlayerToTeam(player, curTime);
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
    
    public void terminateMatches() {
    	for (Match m : matchList) {
    		if (!m.isMatchEnded())
    			m.matchTerminate();
    	}
    	matchList.clear();
    }

}
