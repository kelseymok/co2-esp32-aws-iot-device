resource "aws_iot_thing" "this" {
  name = var.thing-name
}

resource "aws_iot_certificate" "this" {
  active = true
}

resource "aws_iot_thing_principal_attachment" "this" {
  principal = aws_iot_certificate.this.arn
  thing     = aws_iot_thing.this.name
}

resource "aws_iot_policy_attachment" "this" {
  policy = aws_iot_policy.this.name
  target = aws_iot_certificate.this.arn
}

resource "aws_iot_policy" "this" {
  policy = data.aws_iam_policy_document.this.json
  name   = var.thing-name
}

data "aws_iam_policy_document" "this" {
  statement {
    actions = ["iot:Connect"]
    resources = [
      "arn:aws:iot:${data.aws_region.current.name}:${data.aws_caller_identity.current.account_id}:client/${var.thing-name}"
    ]
  }

  statement {
    actions = [
      "iot:Subscribe"
    ]
    resources = [
      "arn:aws:iot:${data.aws_region.current.name}:${data.aws_caller_identity.current.account_id}:topicfilter/${var.thing-name}/sub"
    ]
  }

  statement {
    actions = [
      "iot:Receive"
    ]
    resources = [
      "arn:aws:iot:${data.aws_region.current.name}:${data.aws_caller_identity.current.account_id}:topic/${var.thing-name}/sub"
    ]
  }

  statement {
    actions = [
      "iot:Publish"
    ]
    resources = [
      "arn:aws:iot:${data.aws_region.current.name}:${data.aws_caller_identity.current.account_id}:topic/${var.thing-name}/pub"
    ]
  }
}

// Consider using Basic Ingest: https://docs.aws.amazon.com/iot/latest/developerguide/iot-basic-ingest.html
//  $aws/rules/rule_name/*