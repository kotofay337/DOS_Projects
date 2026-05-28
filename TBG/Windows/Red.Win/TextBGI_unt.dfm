object TextBGI: TTextBGI
  Left = 142
  Top = 319
  BorderIcons = [biSystemMenu, biHelp]
  BorderStyle = bsSingle
  Caption = #1058#1077#1082#1089#1090' BGI'
  ClientHeight = 34
  ClientWidth = 608
  Color = clBtnFace
  DefaultMonitor = dmMainForm
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poDefault
  Visible = True
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 13
  object TextBGIEdit: TEdit
    Left = 85
    Top = 2
    Width = 436
    Height = 26
    Hint = #1058#1077#1082#1089#1090' '#1076#1083#1103' '#1074#1099#1074#1086#1076#1072' BGI '#1096#1088#1080#1092#1090#1072#1084#1080
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = 5921370
    Font.Height = -16
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 0
    OnChange = TextBGIEditChange
    OnKeyPress = TextBGIEditKeyPress
  end
  object Edit1: TEdit
    Left = 526
    Top = 3
    Width = 57
    Height = 26
    Hint = #1053#1072#1078#1084#1080#1090#1077' Enter '#1076#1083#1103' '#1074#1074#1086#1076#1072' '#1091#1075#1083#1072
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 1
    Text = '360.8'
    OnKeyPress = Edit1KeyPress
  end
  object UpDown1: TUpDown
    Left = 585
    Top = 3
    Width = 17
    Height = 27
    Hint = #1059#1075#1086#1083' '#1087#1086#1074#1086#1088#1086#1090#1072' '#1090#1077#1082#1089#1090#1072
    Min = 0
    Max = 3600
    ParentShowHint = False
    Position = 0
    ShowHint = True
    TabOrder = 2
    Wrap = True
    OnClick = UpDown1Click
  end
  object Edit2: TEdit
    Left = 4
    Top = 3
    Width = 57
    Height = 26
    Hint = #1053#1072#1078#1084#1080#1090#1077' Enter '#1076#1083#1103' '#1074#1074#1086#1076#1072' '#1091#1075#1083#1072
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 3
    Text = '4'
    OnKeyPress = Edit2KeyPress
  end
  object UpDown2: TUpDown
    Left = 61
    Top = 2
    Width = 17
    Height = 27
    Hint = #1059#1075#1086#1083' '#1087#1086#1074#1086#1088#1086#1090#1072' '#1090#1077#1082#1089#1090#1072
    Min = 0
    Max = 3600
    ParentShowHint = False
    Position = 0
    ShowHint = True
    TabOrder = 4
    Wrap = True
    OnClick = UpDown2Click
  end
end
