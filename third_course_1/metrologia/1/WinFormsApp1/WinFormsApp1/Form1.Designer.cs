namespace WinFormsApp1
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            lblResults = new Label();
            txtN1 = new Label();
            txtN2 = new Label();
            textn1 = new Label();
            textn2 = new Label();
            txtVocabulary = new Label();
            txtLength = new Label();
            txtVolume = new Label();
            button1 = new Button();
            dataGridViewOperands = new DataGridView();
            dataGridViewOperators = new DataGridView();
            ((System.ComponentModel.ISupportInitialize)dataGridViewOperands).BeginInit();
            ((System.ComponentModel.ISupportInitialize)dataGridViewOperators).BeginInit();
            SuspendLayout();
            // 
            // lblResults
            // 
            lblResults.Location = new Point(0, 0);
            lblResults.Name = "lblResults";
            lblResults.Size = new Size(100, 23);
            lblResults.TabIndex = 1;
            // 
            // txtN1
            // 
            txtN1.AutoSize = true;
            txtN1.Location = new Point(166, 563);
            txtN1.Name = "txtN1";
            txtN1.Size = new Size(197, 20);
            txtN1.TabIndex = 3;
            txtN1.Text = "Общее число операторов: ";
            // 
            // txtN2
            // 
            txtN2.AutoSize = true;
            txtN2.Location = new Point(166, 605);
            txtN2.Name = "txtN2";
            txtN2.Size = new Size(190, 20);
            txtN2.TabIndex = 4;
            txtN2.Text = "Общее число операндов: ";
            // 
            // textn1
            // 
            textn1.AutoSize = true;
            textn1.Location = new Point(166, 650);
            textn1.Name = "textn1";
            textn1.Size = new Size(163, 20);
            textn1.TabIndex = 5;
            textn1.Text = "Словарь операторов: ";
            // 
            // textn2
            // 
            textn2.AutoSize = true;
            textn2.Location = new Point(776, 563);
            textn2.Name = "textn2";
            textn2.Size = new Size(156, 20);
            textn2.TabIndex = 6;
            textn2.Text = "Словарь операндов: ";
            // 
            // txtVocabulary
            // 
            txtVocabulary.AutoSize = true;
            txtVocabulary.Location = new Point(776, 607);
            txtVocabulary.Name = "txtVocabulary";
            txtVocabulary.Size = new Size(162, 20);
            txtVocabulary.TabIndex = 7;
            txtVocabulary.Text = "Словарь программы: ";
            // 
            // txtLength
            // 
            txtLength.AutoSize = true;
            txtLength.Location = new Point(776, 648);
            txtLength.Name = "txtLength";
            txtLength.Size = new Size(147, 20);
            txtLength.TabIndex = 8;
            txtLength.Text = "Длина программы: ";
            // 
            // txtVolume
            // 
            txtVolume.AutoSize = true;
            txtVolume.Location = new Point(776, 689);
            txtVolume.Name = "txtVolume";
            txtVolume.Size = new Size(151, 20);
            txtVolume.TabIndex = 9;
            txtVolume.Text = "Объем программы: ";
            // 
            // button1
            // 
            button1.Location = new Point(166, 689);
            button1.Name = "button1";
            button1.Size = new Size(94, 29);
            button1.TabIndex = 10;
            button1.Text = "Рассчитать";
            button1.UseVisualStyleBackColor = true;
            button1.Click += button1_Click;
            // 
            // dataGridViewOperands
            // 
            dataGridViewOperands.ColumnHeadersHeightSizeMode = DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            dataGridViewOperands.Location = new Point(685, 26);
            dataGridViewOperands.Name = "dataGridViewOperands";
            dataGridViewOperands.RowHeadersWidth = 51;
            dataGridViewOperands.Size = new Size(628, 522);
            dataGridViewOperands.TabIndex = 11;
            // 
            // dataGridViewOperators
            // 
            dataGridViewOperators.ColumnHeadersHeightSizeMode = DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            dataGridViewOperators.Location = new Point(12, 26);
            dataGridViewOperators.Name = "dataGridViewOperators";
            dataGridViewOperators.RowHeadersWidth = 51;
            dataGridViewOperators.Size = new Size(628, 522);
            dataGridViewOperators.TabIndex = 12;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(8F, 20F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(1325, 770);
            Controls.Add(dataGridViewOperators);
            Controls.Add(dataGridViewOperands);
            Controls.Add(button1);
            Controls.Add(txtVolume);
            Controls.Add(txtLength);
            Controls.Add(txtVocabulary);
            Controls.Add(textn2);
            Controls.Add(textn1);
            Controls.Add(txtN2);
            Controls.Add(txtN1);
            Controls.Add(lblResults);
            Name = "Form1";
            Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)dataGridViewOperands).EndInit();
            ((System.ComponentModel.ISupportInitialize)dataGridViewOperators).EndInit();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Label lblResults;
        private Label txtN1;
        private Label txtN2;
        private Label textn1;
        private Label textn2;
        private Label txtVocabulary;
        private Label txtLength;
        private Label txtVolume;
        private Button button1;
        private DataGridView dataGridViewOperands;
        private DataGridView dataGridViewOperators;
    }
}
