class ProgressBar {
  public:
    ProgressBar(LiquidCrystal *lcd);
    void write(float percentage, int width);

  private:
    LiquidCrystal *lcd;
    void configureChars();
};
