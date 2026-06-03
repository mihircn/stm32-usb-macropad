APP_STYLE = """
QWidget {
    background: #101216;
    color: #f1f5f9;
    font-family: 'Segoe UI';
    font-size: 13px;
}
QMainWindow { background: #101216; }
QFrame#Panel, QFrame#Card {
    background: #171b22;
    border: 1px solid #272e3a;
    border-radius: 14px;
}
QLabel#Title {
    font-size: 22px;
    font-weight: 700;
}
QLabel#Muted {
    color: #94a3b8;
}
QPushButton {
    background: #232a35;
    border: 1px solid #334155;
    border-radius: 10px;
    padding: 10px 16px;
}
QPushButton:hover { background: #2b3441; }
QPushButton:disabled {
    color: #6b7280;
    background: #1b2028;
}
QLineEdit, QTextEdit, QComboBox {
    background: #0f141c;
    border: 1px solid #273243;
    border-radius: 10px;
    padding: 8px;
}
QTableWidget {
    background: #0f141c;
    border: 1px solid #273243;
    border-radius: 10px;
    gridline-color: #273243;
}
QHeaderView::section {
    background: #1b2330;
    border: none;
    border-bottom: 1px solid #273243;
    padding: 8px;
    font-weight: 600;
}
"""
