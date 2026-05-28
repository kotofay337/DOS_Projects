object ZoomScreen: TZoomScreen
  Left = 60
  Top = 69
  Cursor = crHandPoint
  BorderIcons = [biHelp]
  BorderStyle = bsNone
  Caption = #1069#1082#1088#1072#1085#1085#1072#1103' '#1083#1091#1087#1072
  ClientHeight = 236
  ClientWidth = 231
  Color = clBtnFace
  DefaultMonitor = dmMainForm
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnMouseDown = FormMouseDown
  OnMouseMove = FormMouseMove
  OnMouseUp = FormMouseUp
  PixelsPerInch = 96
  TextHeight = 13
  object Timer1: TTimer
    Enabled = False
    Interval = 25
    OnTimer = Timer1Timer
  end
end
