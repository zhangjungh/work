package com.riotgames.interview.hongkong.matchmaking;

public interface Matchmaker {

    /**
     * <p>
     * Find a match with the given number of players per team.
     * </p>
     * 
     * @param playersPerTeam
     *            the number of players required in each team
     * @param player
     *            the player
     * @return an appropriate match or null if there is no appropriate match
     */
    Match findMatch(int playersPerTeam, Player player);

    /**
     * <p>
     * Add a player with number per team for matching.
     * </p>
     */
    void enterMatchmaking(int playersPerTeam, Player player);

    /**
     * <p>
     * Update all matches to set start or end status
     * </p>
     */    
    void updateMatchList(long curTime);

    /**
     * <p>
     * terminate all unfinished matches and waiting matches
     * </p>
     */        
    void terminateMatches();
}
