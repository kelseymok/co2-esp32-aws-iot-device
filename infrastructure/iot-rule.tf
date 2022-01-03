resource "aws_iot_topic_rule" "rule" {
  name        = "${var.thing-name}Rule"
  description = "${var.thing-name}Rule"
  enabled     = true
  sql         = "SELECT * FROM '${var.thing-name}/pub'"
  sql_version = "2016-03-23"

  s3 {
    bucket_name = aws_s3_bucket.this.id
    key         = "$${topic()}/$${timestamp()}"
    role_arn    = aws_iam_role.rule.arn
  }
}

resource "aws_iam_role" "rule" {
  name = "${var.thing-name}Rule"
  assume_role_policy = data.aws_iam_policy_document.rule-assume-role.json
  force_detach_policies = true
}

data "aws_iam_policy_document" "rule-assume-role" {
  statement {
    principals {
      identifiers = ["iot.amazonaws.com"]
      type        = "Service"
    }
    actions = [
      "sts:AssumeRole"
    ]
#    condition {
#      test     = "StringEquals"
#      values   = [
#        data.aws_caller_identity.current.account_id
#      ]
#      variable = "aws:SourceAccount"
#    }
#    condition {
#      test     = "ArnLike"
#      values   = [
#        "arn:aws:iot:${data.aws_region.current.name}:${data.aws_caller_identity.current.account_id}:*"
#      ]
#      variable = "aws:SourceArn"
#    }
  }
}

resource "aws_iam_role_policy_attachment" "rule" {
  policy_arn = aws_iam_policy.rule.arn
  role       = aws_iam_role.rule.id
}

resource "aws_iam_policy" "rule" {
  policy = data.aws_iam_policy_document.rule.json
}

data "aws_iam_policy_document" "rule" {
  statement {
    actions = [
      "s3:PutObject"
    ]
    resources = [
      "${aws_s3_bucket.this.arn}/*"
    ]
  }
}