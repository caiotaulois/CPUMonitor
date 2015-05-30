/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package cpumonitor;

import java.io.IOException;
import java.io.StringReader;
import java.util.Collections;
import java.util.HashSet;
import java.util.Set;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.websocket.OnClose;
import javax.websocket.OnOpen;
import javax.websocket.Session;
import javax.websocket.server.ServerEndpoint;
import javax.json.Json;
import javax.json.JsonException;
import javax.json.JsonObject;
/**
 *
 * @author Caio
 */
@ServerEndpoint("/input")
public class Input extends HttpServlet{
    private static Set<Session> peers = Collections.synchronizedSet(new HashSet<Session>());

    private static Session peer1;
    
    @OnOpen
    public void onOpen(Session peer) {
        peers.add(peer);  
        peer1 = peer;
    }
    
    @OnClose
    public void onClose(Session peer) {
        peers.remove(peer);
    }
    
    public void run(String cpuJson){
        try {
            peer1.getBasicRemote().sendText(cpuJson);
        } catch (Exception e) {
            System.out.println("--- Exception: "+e);
        }
    }
    
    protected void processRequest(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {      
        request.setCharacterEncoding("UTF8");
        response.setCharacterEncoding("UTF8");
        try {
            JsonObject cpuJson = Json.createReader(new StringReader(request.getParameter("json"))).readObject();            
            System.out.println(cpuJson);
            run(cpuJson.toString());
        } catch (Exception e) {}
        
    }

    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        processRequest(request, response);
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        processRequest(request, response);
    }

    @Override
    public String getServletInfo() {
        return "Short description";
    }
}
