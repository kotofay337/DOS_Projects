object TestForm: TTestForm
  Left = 62
  Top = 150
  Width = 783
  Height = 540
  Caption = 'TestForm'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnDestroy = FormDestroy
  OnPaint = FormPaint
  PixelsPerInch = 96
  TextHeight = 13
  object Button1: TButton
    Left = 24
    Top = 16
    Width = 57
    Height = 25
    Caption = 'Load pic'
    TabOrder = 0
    OnClick = Button1Click
  end
  object CheckBoxSmooth: TCheckBox
    Left = 24
    Top = 40
    Width = 97
    Height = 17
    Caption = 'Smooth'
    TabOrder = 1
  end
  object OpenDialog1: TOpenDialog
    Left = 24
    Top = 56
  end
end
