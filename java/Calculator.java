import java.awt.Color;
import java.awt.Cursor;
import java.awt.Font;
import java.awt.Rectangle;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.regex.Pattern;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JTextField;

class Calculator extends JFrame {

	private static final int WINDOW_WIDTH = 360;
	private static final int WINDOW_HEIGHT = 540;
	private static final int BUTTON_WIDTH = 70;
	private static final int BUTTON_HEIGHT = 60;
	private static final int MARGIN_X = 25;
	private static final int MARGIN_Y = 25;

	private final int x[] = { MARGIN_X, MARGIN_X + BUTTON_WIDTH + 10, MARGIN_X + (BUTTON_WIDTH + 10) * 2,
			MARGIN_X + (BUTTON_WIDTH + 10) * 3 },
			y[] = { MARGIN_Y + 100, MARGIN_Y + 100 + 75, MARGIN_Y + 100 + 75 * 2, MARGIN_Y + 100 + 75 * 3,
					MARGIN_Y + 100 + 75 * 4 };
	private final Rectangle numberButtonMapping[] = { new Rectangle(x[1], y[4], BUTTON_WIDTH, BUTTON_HEIGHT), // 0
			new Rectangle(x[0], y[3], BUTTON_WIDTH, BUTTON_HEIGHT), // 1
			new Rectangle(x[1], y[3], BUTTON_WIDTH, BUTTON_HEIGHT), // 2
			new Rectangle(x[2], y[3], BUTTON_WIDTH, BUTTON_HEIGHT), // 3
			new Rectangle(x[0], y[2], BUTTON_WIDTH, BUTTON_HEIGHT), // 4
			new Rectangle(x[1], y[2], BUTTON_WIDTH, BUTTON_HEIGHT), // 5
			new Rectangle(x[2], y[2], BUTTON_WIDTH, BUTTON_HEIGHT), // 6
			new Rectangle(x[0], y[1], BUTTON_WIDTH, BUTTON_HEIGHT), // 7
			new Rectangle(x[1], y[1], BUTTON_WIDTH, BUTTON_HEIGHT), // 8
			new Rectangle(x[2], y[1], BUTTON_WIDTH, BUTTON_HEIGHT) }; // 9

	private JTextField display; // Input text
	private JButton buttonAdd, buttonSub, buttonMul, buttonDiv, buttonPoint, buttonClear, buttonBack, buttonMod,
			buttonEqual;
	private JButton numberButton[] = new JButton[10];

	private int i;
	private char option = ' ';
	private double accumulator = 0;
	private boolean isOptionClicked = false;

	private boolean isDividedByZero = false;

	private Calculator() {
		setTitle("Calculator");
		setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
		setLocationRelativeTo(null);

		display = new JTextField("0");
		display.setBounds(25, 25, 310, 75);
		display.setEditable(false);
		display.setBackground(Color.WHITE);
		display.setForeground(Color.darkGray);
		display.setFont(new Font("Comic Sans MS", Font.PLAIN, 33));
		add(display);

		// number buttons initicalization
		for (i = 0; i < 10; i++) {
			numberButton[i] = new JButton(String.valueOf(i));
			numberButton[i].setBounds(numberButtonMapping[i]);
			numberButton[i].setCursor(new Cursor(Cursor.HAND_CURSOR));
			int digit = i;
			numberButton[i].addActionListener(event -> {
				insert((char) (digit + '0'));
			});
			add(numberButton[i]);
		}

		buttonPoint = new JButton(".");
		buttonPoint.setBounds(x[0], y[4], BUTTON_WIDTH, BUTTON_HEIGHT);
		buttonPoint.setCursor(new Cursor(Cursor.HAND_CURSOR));
		buttonPoint.addActionListener(event -> {
			insert('.');
			isOptionClicked = false;
		});
		add(buttonPoint);

		buttonClear = new JButton("C");
		buttonClear.setBounds(x[0], y[0], BUTTON_WIDTH, BUTTON_HEIGHT);
		buttonClear.setCursor(new Cursor(Cursor.HAND_CURSOR));
		buttonClear.addActionListener(event -> {
			display.setText("0");
			accumulator = 0;
			isOptionClicked = false;
			isDividedByZero = false;
			option = ' ';
		});
		add(buttonClear);

		buttonBack = new JButton("<-");
		buttonBack.setBounds(x[1], y[0], BUTTON_WIDTH, BUTTON_HEIGHT);
		buttonBack.setCursor(new Cursor(Cursor.HAND_CURSOR));
		buttonBack.addActionListener(event -> {
			delete();
		});
		add(buttonBack);

		buttonMod = new JButton("%");
		buttonMod.setBounds(x[2], y[0], BUTTON_WIDTH, BUTTON_HEIGHT);
		buttonMod.setCursor(new Cursor(Cursor.HAND_CURSOR));
		buttonMod.addActionListener(event -> {
			calculate('%');
		});
		add(buttonMod);

		buttonAdd = new JButton("+");
		buttonAdd.setBounds(x[3], y[3], BUTTON_WIDTH, BUTTON_HEIGHT);
		buttonAdd.setCursor(new Cursor(Cursor.HAND_CURSOR));
		buttonAdd.addActionListener(event -> {
			calculate('+');
		});
		add(buttonAdd);

		buttonSub = new JButton("-");
		buttonSub.setBounds(x[3], y[2], BUTTON_WIDTH, BUTTON_HEIGHT);
		buttonSub.setCursor(new Cursor(Cursor.HAND_CURSOR));
		buttonSub.addActionListener(event -> {
			calculate('-');
		});
		add(buttonSub);

		buttonMul = new JButton("x");
		buttonMul.setBounds(x[3], y[1], BUTTON_WIDTH, BUTTON_HEIGHT);
		buttonMul.setCursor(new Cursor(Cursor.HAND_CURSOR));
		buttonMul.addActionListener(event -> {
			calculate('x');
		});
		add(buttonMul);

		buttonDiv = new JButton("/");
		buttonDiv.setBounds(x[3], y[0], BUTTON_WIDTH, BUTTON_HEIGHT);
		buttonDiv.setCursor(new Cursor(Cursor.HAND_CURSOR));
		buttonDiv.addActionListener(event -> {
			calculate('/');
		});
		add(buttonDiv);

		buttonEqual = new JButton("=");
		buttonEqual.setBounds(x[2], y[4], BUTTON_WIDTH * 2 + 10, BUTTON_HEIGHT);
		buttonEqual.setCursor(new Cursor(Cursor.HAND_CURSOR));
		buttonEqual.addActionListener(event -> {
			calculate('=');
		});
		addKeyListener(keyListener);
		add(buttonEqual);

		setLayout(null);
		setResizable(false);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setVisible(true);
	}

	public static void main(String[] args) {
		new Calculator().setVisible(true);
	}

	KeyAdapter keyListener = new KeyAdapter() {
		@Override
		public void keyTyped(KeyEvent e) {
			if (e.getKeyChar() == '0') {
				// delete row method (when "delete" is pressed)
				System.out.println("Key \"Delete\" Pressed");
			}
		}

		@Override
		public void keyPressed(KeyEvent e) {
			if (e.getKeyChar() == '0') {
				// delete row method (when "delete" is pressed)
				System.out.println("Key \"Delete\" Pressed");
			}
		}

		@Override
		public void keyReleased(KeyEvent e) {
			if (e.getKeyChar() == '0') {
				// delete row method (when "delete" is released)
				System.out.println("Key \"Delete\" Released");
			}
		}
	};

	private void insert(char character) {
		if (isDividedByZero) {
			return;
		}

		if (isOptionClicked && character != '.') { // replace original text on display
			display.setText(String.valueOf(character));
			isOptionClicked = false;
		} else if (Pattern.matches("[0]*", display.getText()) && character != '.') {
			display.setText(String.valueOf(character));
		} else if (character != '.' || Pattern.matches("^[^<.>]+$", display.getText())) { // don't insert '.' if there's
																							// one on display already
			display.setText(display.getText() + character);
		}
	}

	// TODO: delete both "." and digit if there's only one digit after "."
	// todo implemented. next: use regex
	private void delete() {
		if (isDividedByZero) {
			return;
		}

		String str = display.getText();
		StringBuilder strbuilder = new StringBuilder();

		for (i = 0; i < (str.length()
				- ((str.contains(".") && str.toCharArray()[str.length() - 2] == '.') ? 2 : 1)); i++) {
			strbuilder.append(str.charAt(i));
		}
		str = strbuilder.toString();
		if (str.equals("") || (str.length() == 1 && str.contains("-"))) {
			display.setText("0");
		} else {
			display.setText(str);
		}
		isOptionClicked = false;
	}

	private void calculate(char option) {
		if (isDividedByZero) {
			return;
		}
		// prevent accumulator from been updated if an option key has been continuously
		// clicked multiple times
		if (!isOptionClicked) {
			updateAccumulator();
		}
		updateDisplay();
		this.option = option;
		isOptionClicked = true;
	}

	private void updateDisplay() {
		if (isDividedByZero) { // divition by zero
			display.setText("DIVITION BY 0");
		} else if (Math.ceil(accumulator) == accumulator) {
			display.setText(String.valueOf((int) accumulator));
		} else {
			display.setText(String.valueOf(accumulator));
		}
	}

	private void updateAccumulator() {
		switch (option) {
			case '+':
				accumulator += Double.valueOf(display.getText());
				break;
			case '-':
				accumulator -= Double.valueOf(display.getText());
				break;
			case 'x':
				accumulator *= Double.valueOf(display.getText());
				break;
			case '/':
				if (Double.valueOf(display.getText()) == 0) { // divition by zero
					isDividedByZero = true;
					break;
				}
				accumulator /= Double.valueOf(display.getText());
				break;
			case '%':
				if (Double.valueOf(display.getText()) == 0) { // divition by zero
					isDividedByZero = true;
					break;
				}
				accumulator %= Double.valueOf(display.getText());
				break;
			default:
				accumulator = Double.valueOf(display.getText());
				break;
		}
		option = ' ';
	}
}
