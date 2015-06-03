/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package cpumonitor;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.util.ArrayList;
import javax.json.JsonObject;

public class databaseDAO extends BaseDAO {

    public databaseDAO() {
    }

    //--------------------------------------------------------------------------
    public void insert(JsonObject json) {
        try {
            Connection con = getConnection();
            PreparedStatement pstmt = con.prepareStatement(
               "INSERT INTO cpumonitor (date, hostname, memoryusage, cpuusage) VALUES(?,?,?,?)");
            pstmt.setString(1, json.getString("date"));
            pstmt.setString(2, json.getString("hostname"));
            pstmt.setString(3, json.getString("memoryUsage"));
            pstmt.setString(4, json.getString("cpuUsage"));
            pstmt.execute();
            con.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
