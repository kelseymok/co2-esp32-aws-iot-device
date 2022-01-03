output "device-certificate" {
  value = aws_iot_certificate.this.certificate_pem
  sensitive = true
}

output "device-private-key" {
  value = aws_iot_certificate.this.private_key
  sensitive = true
}

data "aws_iot_endpoint" "this" {}

output "iot-endpoint-address" {
  value = data.aws_iot_endpoint.this.endpoint_address
  sensitive = true
}

output "thing-name" {
  value = var.thing-name
}