class CustomChars {
  public:
    CustomChars(LiquidCrystal *lcd);
    void progressBar(int column, float percentage);
    void smile(int column, int row);
    void level(int column, int row);
    void frown(int column, int row);
  private:
    LiquidCrystal *lcd;
    void configureChars();
};
