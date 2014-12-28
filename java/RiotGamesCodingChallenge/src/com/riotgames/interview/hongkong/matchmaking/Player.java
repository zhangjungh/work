package com.riotgames.interview.hongkong.matchmaking;

/**
 * <p>
 * Representation of a player.
 * </p>
 * <p>
 * As indicated in the challenge description, feel free to augment the Player
 * class in any way that you feel will improve your final matchmaking solution.
 * <strong>Do NOT remove the name, wins, or losses fields.</strong> Also note
 * that if you make any of these changes, you are responsible for updating the
 * {@link SampleData} such that it provides a useful data set to exercise your
 * solution.
 * </p>
 */
public class Player {
    private final String name;
    private long wins;
    private long losses;
    private double rank;

    public Player(String name, long wins, long losses) {
        this.name = name;
        this.wins = wins;
        this.losses = losses;
        //calculate the Elo rank if not sets
        this.rank = Config.getInitRank() + (getWinRate() - 0.5) * Config.getRange();       
    }

    public Player(String name, long wins, long losses, double rank) {
        this.name = name;
        this.wins = wins;
        this.losses = losses;
        this.rank = rank;
    }
    
    public String getName() {
        return name;
    }

    public long getWins() {
        return wins;
    }

    public long getLosses() {
        return losses;
    }
    
    private double getWinRate() {
    	if (wins == 0)
    		return 0.0;
    	else
    		return (double)wins / (wins + losses);
    }

    public double getRank() {
    	return rank;
    }
    
    public void setMatchResult(boolean bWin, double deltaRank) {
    	if (bWin)
    		wins += 1;
    	else
    		losses += 1;
    	rank += deltaRank;
    }
}
