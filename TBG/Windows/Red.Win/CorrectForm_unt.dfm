object CorrectForm: TCorrectForm
  Left = 170
  Top = 0
  BorderIcons = [biSystemMenu, biHelp]
  BorderStyle = bsSingle
  Caption = #1050#1086#1088#1088#1077#1082#1090#1080#1088#1086#1074#1082#1072
  ClientHeight = 122
  ClientWidth = 487
  Color = clBtnFace
  DefaultMonitor = dmMainForm
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 145
    Height = 105
    Caption = #1054#1073#1097#1080#1077
    TabOrder = 0
    object CheckBox1: TCheckBox
      Tag = 1
      Left = 16
      Top = 16
      Width = 121
      Height = 17
      Hint = #1056#1077#1076#1072#1082#1090#1080#1088#1086#1074#1072#1085#1080#1077' '#1082#1086#1086#1088#1076#1080#1085#1072#1090' '#1091#1079#1083#1086#1074' '#1043#1055
      Caption = #1050#1086#1086#1088#1076#1080#1085#1072#1090#1099' '#1091#1079#1083#1086#1074
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
      OnClick = CheckBoxClick
    end
  end
  object GroupBox2: TGroupBox
    Left = 160
    Top = 8
    Width = 81
    Height = 105
    Caption = #1051#1080#1085#1080#1080
    TabOrder = 1
    object cbxLineStyle: TCheckBox
      Tag = 11
      Left = 8
      Top = 16
      Width = 57
      Height = 17
      Hint = #1050#1086#1088#1088#1077#1082#1090#1091#1088#1072' '#1089#1090#1080#1083#1103' '#1083#1080#1085#1080#1080
      Caption = #1057#1090#1080#1083#1100
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
      OnClick = CheckBoxClick
    end
    object cbxLineColor: TCheckBox
      Tag = 12
      Left = 8
      Top = 32
      Width = 49
      Height = 17
      Hint = #1050#1086#1088#1088#1077#1082#1090#1091#1088#1072' '#1094#1074#1077#1090#1072' '#1083#1080#1085#1080#1080
      Caption = #1062#1074#1077#1090
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      OnClick = CheckBoxClick
    end
    object cbxLineThick: TCheckBox
      Tag = 13
      Left = 8
      Top = 48
      Width = 65
      Height = 17
      Hint = #1050#1086#1088#1088#1077#1082#1090#1091#1088#1072' '#1090#1086#1083#1097#1080#1085#1099' '#1083#1085#1080#1080
      Caption = #1058#1086#1083#1097#1080#1085#1072
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      OnClick = CheckBoxClick
    end
  end
  object GroupBox3: TGroupBox
    Left = 248
    Top = 8
    Width = 89
    Height = 105
    Caption = #1047#1072#1087#1086#1083#1085#1077#1085#1080#1077
    TabOrder = 2
    object cbxFillStyle: TCheckBox
      Tag = 21
      Left = 8
      Top = 16
      Width = 57
      Height = 17
      Hint = #1050#1086#1088#1088#1077#1082#1090#1091#1088#1072' '#1089#1090#1080#1083#1103' '#1079#1072#1083#1080#1074#1082#1080
      Caption = #1057#1090#1080#1083#1100
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
      OnClick = CheckBoxClick
    end
    object cbxFillColor: TCheckBox
      Tag = 22
      Left = 8
      Top = 32
      Width = 49
      Height = 17
      Hint = #1050#1086#1088#1088#1077#1082#1090#1091#1088#1072' '#1094#1074#1077#1090#1072' '#1079#1072#1083#1080#1074#1082#1080
      Caption = #1062#1074#1077#1090
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      OnClick = CheckBoxClick
    end
    object CheckBox9: TCheckBox
      Tag = 23
      Left = 8
      Top = 48
      Width = 73
      Height = 17
      Hint = #1050#1086#1088#1088#1077#1082#1090#1091#1088#1072' '#1094#1074#1077#1090#1072' '#1092#1086#1085#1072' '#1079#1072#1083#1080#1074#1082#1080
      Caption = #1062#1074#1077#1090' '#1092#1086#1085#1072
      Enabled = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      OnClick = CheckBoxClick
    end
  end
  object GroupBox4: TGroupBox
    Left = 344
    Top = 8
    Width = 137
    Height = 105
    Caption = #1058#1077#1082#1089#1090
    TabOrder = 3
    object cbxTextStyle: TCheckBox
      Tag = 31
      Left = 8
      Top = 32
      Width = 97
      Height = 17
      Hint = #1050#1086#1088#1088#1077#1082#1090#1091#1088#1072' '#1085#1072#1095#1077#1088#1090#1072#1085#1080#1103' '#1090#1077#1082#1089#1090#1072
      Caption = #1053#1072#1095#1077#1088#1090#1072#1085#1080#1077
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
      OnClick = CheckBoxClick
    end
    object cbxTextColor: TCheckBox
      Tag = 32
      Left = 8
      Top = 48
      Width = 96
      Height = 17
      Hint = #1050#1086#1088#1088#1077#1082#1090#1091#1088#1072' '#1094#1074#1077#1090#1072' '#1090#1077#1082#1089#1090#1072
      Caption = #1062#1074#1077#1090
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      OnClick = CheckBoxClick
    end
    object cbxText: TCheckBox
      Tag = 33
      Left = 8
      Top = 64
      Width = 96
      Height = 17
      Hint = #1050#1086#1088#1088#1077#1082#1090#1091#1088#1072' '#1089#1086#1076#1077#1088#1078#1072#1085#1080#1103
      Caption = #1057#1086#1076#1077#1088#1078#1072#1085#1080#1077
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      OnClick = CheckBoxClick
    end
    object CheckBox11: TCheckBox
      Left = 8
      Top = 80
      Width = 121
      Height = 17
      Hint = #1050#1086#1085#1074#1077#1088#1090#1072#1094#1080#1103' '#1074' BGI / TrueType '
      Caption = #1058#1080#1087' (BGI/TrueType)'
      Enabled = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 3
    end
    object cbxTextSize: TCheckBox
      Tag = 31
      Left = 8
      Top = 16
      Width = 97
      Height = 17
      Hint = #1050#1086#1088#1088#1077#1082#1090#1091#1088#1072' '#1085#1072#1095#1077#1088#1090#1072#1085#1080#1103' '#1090#1077#1082#1089#1090#1072
      Caption = #1056#1072#1079#1084#1077#1088
      ParentShowHint = False
      ShowHint = True
      TabOrder = 4
      OnClick = CheckBoxClick
    end
  end
end
