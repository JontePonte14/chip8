public class Main{

	public static void main(String[] args) {
		int grid[][] = new int[5][5];
		for (int i = 0; i < 5; i++){
			grid[0][i] = 1;
		}

		grid[3][3] = 1;

		MyFrame frame = new MyFrame(grid);
	}




}
