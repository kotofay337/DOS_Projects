object Form1: TForm1
  Left = 116
  Top = 151
  Width = 696
  Height = 480
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object OleContainer1: TOleContainer
    Left = 0
    Top = 0
    Width = 688
    Height = 426
    Align = alClient
    Caption = 'OleContainer1'
    TabOrder = 0
  end
  object MainMenu1: TMainMenu
    Left = 64
    Top = 24
    object N1: TMenuItem
      Caption = 'Объект'
      object MNew: TMenuItem
        Caption = 'Новый'
        OnClick = MNewClick
      end
      object MOpen: TMenuItem
        Caption = 'Открыть'
        OnClick = MOpenClick
      end
      object MSave: TMenuItem
        Caption = 'Сохранить'
        OnClick = MSaveClick
      end
      object MClose: TMenuItem
        Caption = 'Закрыть'
        OnClick = MCloseClick
      end
    end
  end
  object OpenDialog1: TOpenDialog
    DefaultExt = 'ole'
    Filter = 'Объекты OLE|*.ole|Все файлы|*.*'
    Left = 104
    Top = 24
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = 'ole'
    Filter = 'Объекты OLE|*.ole|Все файлы|*.*'
    Left = 144
    Top = 24
  end
end
