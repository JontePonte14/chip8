import javax.swing.JFrame;

public class MyFrame extends JFrame{
    MyPanel panel;

    MyFrame(int[][] video) {
        panel = new MyPanel(video);
        this.setTitle("CHIP8 EMULATOR");
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        this.add(panel);
        this.pack();
        this.setLocationRelativeTo(null);
		this.setVisible(true);
    }

}
