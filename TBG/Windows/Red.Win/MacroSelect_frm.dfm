object MacroSelect: TMacroSelect
  Left = 287
  Top = 147
  BorderStyle = bsSingle
  Caption = #1052#1072#1082#1088#1086
  ClientHeight = 434
  ClientWidth = 455
  Color = clBtnFace
  DefaultMonitor = dmMainForm
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object PrevScr: TImage
    Left = 304
    Top = 8
    Width = 137
    Height = 129
  end
  object MTree: TTreeView
    Left = 8
    Top = 8
    Width = 273
    Height = 417
    Cursor = crHandPoint
    DragMode = dmAutomatic
    Indent = 19
    MultiSelect = True
    ReadOnly = True
    TabOrder = 0
    OnChange = MTreeChange
  end
  object Button1: TButton
    Left = 336
    Top = 200
    Width = 73
    Height = 33
    Caption = #1044#1086#1073#1072#1074#1080#1090#1100
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 336
    Top = 295
    Width = 73
    Height = 33
    Caption = #1048#1079#1084#1077#1085#1080#1090#1100
    TabOrder = 2
  end
  object Button3: TButton
    Left = 336
    Top = 247
    Width = 73
    Height = 33
    Caption = #1059#1076#1072#1083#1080#1090#1100
    TabOrder = 3
  end
  object PrevScrEnable: TCheckBox
    Left = 304
    Top = 144
    Width = 97
    Height = 17
    Caption = #1055#1088#1086#1089#1084#1086#1090#1088
    Checked = True
    State = cbChecked
    TabOrder = 4
    OnClick = PrevScrEnableClick
  end
  object Button4: TButton
    Left = 376
    Top = 391
    Width = 73
    Height = 33
    Caption = #1042#1099#1093#1086#1076
    TabOrder = 5
    OnClick = Button4Click
  end
  object Button5: TButton
    Left = 296
    Top = 391
    Width = 73
    Height = 33
    Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
    TabOrder = 6
  end
end
