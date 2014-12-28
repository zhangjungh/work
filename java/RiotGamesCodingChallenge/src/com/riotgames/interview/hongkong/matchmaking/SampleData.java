package com.riotgames.interview.hongkong.matchmaking;

import java.io.*;
import java.util.*;
import java.util.concurrent.locks.*;

public class SampleData {
	static private final String ORIGINAL_FILE = "playerdata.txt";
	static private final String ORI_RANK_FILE = "PlayerDataRank.txt";
	static private final String NEW_RANK_FILE = "NewPlayerDataRank.txt";
	static private ReadWriteLock s_rwl = new ReentrantReadWriteLock(); 
	static private LinkedList<Player> s_players = new LinkedList<Player>();
    
	public static class PlayerComparator implements Comparator<Player> {
  	  @Override
  	  public int compare(Player p1, Player p2) {
  		  return p1.getName().compareTo(p2.getName());
  	  }
    }
    
    private static void readFile(String filename) throws IOException {  	
        FileReader fr = new FileReader(filename);
        BufferedReader br = new BufferedReader(fr);
        String line;
        while ((line=br.readLine()) != null) 
        {
            String[] arr = line.split(",");
            if (arr.length == 3)
            {
            	s_players.add( new Player( arr[0].trim(), new Long(arr[1].trim()), new Long(arr[2].trim())) );
            }
            else if (arr.length == 4)
            {
            	s_players.add( new Player( arr[0].trim(), new Long(arr[1].trim()), new Long(arr[2].trim()), new Double(arr[3].trim()) ) );
            }
        }
        br.close();
        fr.close();
    }
    
    public static void initPlayerData() {
    	if (!s_players.isEmpty())
    		return;
    	
    	try {
    		File f = new File(NEW_RANK_FILE);
    		if (f.exists()) {
    			readFile(NEW_RANK_FILE);
    		}
    		else {
    			readFile(ORIGINAL_FILE);
    			Collections.sort(s_players, new PlayerComparator());
    			writeFile(ORI_RANK_FILE);	
    		}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    }
    
    private static void writeFile(String filename) throws IOException {
        FileWriter fw = new FileWriter(new File(filename));
        BufferedWriter bw = new BufferedWriter(fw);
        for (Player p : s_players) {
            bw.write(p.getName() + ", " + p.getWins() + ", " + p.getLosses() + ", " + String.format("%.2f", p.getRank()) + "\n");
        }
        bw.close();
        fw.close();
    }   
    
    public static void writePlayerData() {
    	try {
    		Collections.sort(s_players, new PlayerComparator());
			writeFile(NEW_RANK_FILE);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    }
	
    public static List<Player> getPlayers() {
        return s_players;
    }
    
	public static Player spawnPlayer() {
    	Player p = null;
    	s_rwl.writeLock().lock();
    	try {
    		if (!s_players.isEmpty()) {
    			int index = (int) (Math.random() * (s_players.size() - 1));
    			p = s_players.remove(index);
    		}    		
    	} finally {
        	s_rwl.writeLock().unlock();
    	}
    	return p;
    }
    
    public static void putPlayersToPool(Set<Player> players) {
    	s_rwl.writeLock().lock();
    	try {
    		for (Player p : players)
    			s_players.add(p);
    	} finally {
        	s_rwl.writeLock().unlock();
    	}
    } 

}
