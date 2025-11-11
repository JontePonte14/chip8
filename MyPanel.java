import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import javax.swing.JPanel;

public class MyPanel extends JPanel {
    int[][] video;


    public MyPanel(int[][] video) {
        //this.setSize(640, 320);
        this.video = video;
        this.setPreferredSize(new Dimension(640, 320));
    }

        
    public void paint(Graphics g) {
		
        Graphics2D g2D = (Graphics2D) g;

        g2D.setPaint(Color.BLUE);
        for (int r = 0; r < 5; r++){
            for (int c = 0; c < 5; c++){
                if (video[r][c] == 1){
                    g2D.fillRect(r*10, c*10, 10, 10);
                }
            }
        }

        // g2D.setPaint(Color.BLUE);
        // g2D.setStroke(new BasicStroke(5));
        // //g2D.drawLine(0, 0, 640, 320);

        // g2D.drawRect(0, 0, 200, 100);
        // g2D.setPaint(Color.PINK);
        // g2D.fillRect(10, 10, 50, 50);
	}
    
}
