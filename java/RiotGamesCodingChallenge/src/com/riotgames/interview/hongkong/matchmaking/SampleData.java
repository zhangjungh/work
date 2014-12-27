package com.riotgames.interview.hongkong.matchmaking;

import java.io.*;
import java.util.*;
import java.util.concurrent.locks.*;

public class SampleData {
	static private ReadWriteLock s_rwl = new ReentrantReadWriteLock(); 
	static private LinkedList<Player> s_players = new LinkedList<Player>();

    public static void initPlayerData() throws IOException {
    	if (!s_players.isEmpty())
    		return;
    	
        FileReader fr = new FileReader("playerdata.txt");
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
